//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/hashlib.h>
#include <pycpp/random.h>
#include <pycpp/lattice/multipart.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/stl/unordered_map.h>
#include <pycpp/string/unicode.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// CONSTANTS
// ---------

/**
 *  \brief Lookup table for common application types.
 */
unordered_map<std::string, std::string> CONTENT_TYPES = {
    // TEXT
    {"css", "text/css"},
    {"csv", "text/csv"},
    {"html", "text/html"},
    {"md", "text/markdown"},
    {"tbt", "text/tab-separated-values"},
    {"txt", "text/plain"},
    {"tsv", "text/tab-separated-values"},
    {"vcard", "text/vcard"},

    // APPLICATION
    {"gz", "application/gzip"},
    {"gzip", "application/gzip"},
    {"doc", "application/msword"},
    {"http", "application/http"},
    {"js", "application/javascript"},
    {"json", "application/json"},
    {"jsn",  "application/json"},
    {"mp4",  "application/mp4"},
    {"mpeg4",  "application/mpeg4"},
    {"ogg",  "application/ogg"},
    {"pdf",  "application/pdf"},
    // PS postscript? powershell? Don't detect.
    {"rtf", "application/rtf"},
    {"sql", "application/sql"},
    {"xml", "application/xml"},

    // MICROSOFT APPLICATION
    {"dot", "application/msword"},
    {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
    {"dotx", "application/vnd.openxmlformats-officedocument.wordprocessingml.template"},
    {"dotm", "application/vnd.ms-word.document.macroEnabled.12"},
    {"dotc", "application/vnd.ms-word.template.macroEnabled.12"},
    {"xls", "application/vnd.ms-excel"},
    {".xlt", "application/vnd.ms-excel"},
    {".xla", "application/vnd.ms-excel"},
    {".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
    {".xltx", "application/vnd.openxmlformats-officedocument.spreadsheetml.template"},
    {".xlsm", "application/vnd.ms-excel.sheet.macroEnabled.12"},
    {".xltm", "application/vnd.ms-excel.template.macroEnabled.12"},
    {".xlam", "application/vnd.ms-excel.addin.macroEnabled.12"},
    {".xlsb", "application/vnd.ms-excel.sheet.binary.macroEnabled.12"},
    {".ppt", "application/vnd.ms-powerpoint"},
    {".pot", "application/vnd.ms-powerpoint"},
    {".pps", "application/vnd.ms-powerpoint"},
    {".ppa", "application/vnd.ms-powerpoint"},
    {".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
    {".potx", "application/vnd.openxmlformats-officedocument.presentationml.template"},
    {".ppsx", "application/vnd.openxmlformats-officedocument.presentationml.slideshow"},
    {".ppam", "application/vnd.ms-powerpoint.addin.macroEnabled.12"},
    {".pptm", "application/vnd.ms-powerpoint.presentation.macroEnabled.12"},
    {".potm", "application/vnd.ms-powerpoint.template.macroEnabled.12"},
    {".ppsm", "application/vnd.ms-powerpoint.slideshow.macroEnabled.12"},
};


// FUNCTIONS
// ---------


std::string get_boundary()
{
    auto bytes = pseudorandom(8);
    string_wrapper view(bytes.data(), bytes.size());
    auto hex = sha1_hash(view).hexdigest();
    return std::string(hex.data(), hex.size());
}


/**
 *  \brief Read file using custom fstream
 */
static std::string read_fstream(const string_wrapper& filename)
{
    ifstream file(filename, ios_base::in | ios_base::binary);
    ostringstream stream;
    stream << file.rdbuf();

    return stream.str();
}


static std::string detect_content_type(const string_wrapper& filename)
{
    std::string suffix(path_splitext(filename)[1]);
    auto it = CONTENT_TYPES.find(suffix);
    if (it != CONTENT_TYPES.end()) {
        return it->second;
    }

    return "";
}


// OBJECTS
// -------


part_value_t::part_value_t(const char* filename, const char* content_type):
    part_value_t(string_wrapper(filename), string_wrapper(content_type))
{}


part_value_t::part_value_t(std::string&& filename, std::string&& content_type):
    filename(forward<std::string>(filename))
{
    if (content_type.empty()) {
        this->content_type_ = detect_content_type(this->filename);
    } else {
        this->content_type_ = move(content_type);
    }
}


part_value_t::part_value_t(const string_wrapper& filename, const string_wrapper& content_type):
    filename(filename)
{
    if (content_type.empty()) {
        this->content_type_ = detect_content_type(this->filename);
    } else {
        this->content_type_ = std::string(content_type);
    }
}


std::string part_value_t::basename() const
{
    return std::string(base_name(filename));
}


std::string part_value_t::name() const
{
    return std::string(path_splitext(basename())[0]);
}


string_wrapper part_value_t::content_type() const
{
    return string_wrapper(content_type_);
}


/**
 *  RFC-7231 clearly states, if the content type is unknown, do not
 *  send it.
 */
std::string part_value_t::string() const
{
    std::string string;
    string += "Content-Disposition: form-data; ";
    string += "name=\"" + name() + "\"; ";
    string += "filename=\"" + basename() + "\"\r\n";

    if (!content_type().empty()) {
       string += "Content-Type: " + std::string(content_type()) + "\r\n";
    }
    string += "\r\n";

    return string;
}


std::string file_value_t::buffer() const
{
    return read_fstream(filename);
}


std::string file_value_t::string() const
{
    std::string string;
    string += part_value_t::string() + buffer() + "\r\n";

    return string;
}


buffer_value_t::buffer_value_t(const char* filename,
                               const char* buffer,
                               const char* content_type):
    buffer_value_t(string_wrapper(filename), string_wrapper(buffer), string_wrapper(content_type))
{}


buffer_value_t::buffer_value_t(std::string&& filename,
                               std::string&& buffer,
                               std::string&& content_type):
    part_value_t(forward<std::string>(filename), forward<std::string>(content_type)),
    buffer_(forward<std::string>(buffer))
{}


buffer_value_t::buffer_value_t(const string_wrapper& filename,
                               const string_wrapper& buffer,
                               const string_wrapper& content_type):
    part_value_t(filename, content_type),
    buffer_(buffer)
{}


std::string buffer_value_t::buffer() const
{
    return buffer_;
}


std::string buffer_value_t::string() const
{
    std::string string;
    string += part_value_t::string() + buffer() + "\r\n";

    return string;
}

}   /* detail */


multipart_t::multipart_t(initializer_list<typename multipart_t::value_type> list)
{
    for (auto it = list.begin(); it != list.end(); ++it) {
        add(*it);
    }
}


void multipart_t::add(const part_ptr_t& part)
{
    push_back(part);
}


void multipart_t::add(part_ptr_t&& part)
{
    emplace_back(forward<part_ptr_t>(part));
}


string_wrapper multipart_t::boundary() const
{
    return string_wrapper(boundary_);
}


std::string multipart_t::string() const
{
    std::string string;
    for (const auto &item: *this) {
        string += "--" + std::string(boundary()) + "\r\n" + item->string();
    }

    // if any elements were written, write a trailing separator.
    if (*this) {
        string += "--" + std::string(boundary()) + "--\r\n";
    }

    return string;
}


std::string multipart_t::header() const
{
    return "multipart/form-data; boundary=" + std::string(boundary());
}


multipart_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
