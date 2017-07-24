//  :copyright: (c) 2015 Huu Nguyen.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/hashlib.h>
#include <pycpp/random.h>
#include <pycpp/lattice/multipart.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/string/unicode.h>
#include <fstream>
#include <sstream>
#include <unordered_map>

PYCPP_BEGIN_NAMESPACE

namespace detail
{
// CONSTANTS
// ---------

/**
 *  \brief Lookup table for common application types.
 */
std::unordered_map<std::string, std::string> CONTENT_TYPES = {
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
    secure_string_view view(bytes.data(), bytes.size());
    auto hex = sha1_hash(view).hexdigest();
    return std::string(hex.data(), hex.size());
}


/**
 *  \brief Read a file on a POSIX system.
 *
 *  POSIX systems require a null terminating byte for filenames, so
 *  all Unicode-supporting POSIX systems typically use UTF8 for
 *  filename encoding. This is easy, since it's our internal
 *  representation.
 */
static std::string read_narrow(const std::string &filename)
{
    auto *name = filename.data();
    std::ifstream file(name, std::ios_base::in | std::ios_base::binary);
    std::ostringstream stream;
    stream << file.rdbuf();

    return stream.str();
}


/**
 *  \brief Read a file on a Win32 system.
 *
 *  Windows systems support two APIs: a legacy, narrow API, for
 *  backwards compatibility with the local code page, and a
 *  UTF-16 wide API. Although MSVC provides a wide character
 *  overload for std::ifstream, MinGW does not, so the file must
 *  be opened with std::wifstream.
 */
static std::string read_wide(const std::string &filename)
{
    if (!is_unicode(filename)) {
        // ascii only filename, narrow API works.
        return read_narrow(filename);
    }

    auto utf16 = utf8_to_utf16(filename);
    auto *name = reinterpret_cast<const wchar_t*>(utf16.data());
    std::wifstream file(filename, std::ios_base::in | std::ios_base::binary);
    std::wostringstream stream;
    stream << file.rdbuf();

    std::wstring wide = stream.str();
    const size_t size = wide.size() * sizeof(char) / sizeof(wchar_t);
    std::string string(reinterpret_cast<const char*>(wide.data()), size);

    return utf16_to_utf8(string);
}


static std::string detect_content_type(const std::string &filename)
{
    std::string suffix = splitext(filename)[1];
    auto it = CONTENT_TYPES.find(suffix);
    if (it != CONTENT_TYPES.end()) {
        return it->second;
    }

    return "";
}


// OBJECTS
// -------


part_value_t::part_value_t(const std::string& filename, const std::string& content_type):
    filename(filename)
{
    if (content_type.empty()) {
        this->content_type_ = detect_content_type(this->filename);
    } else {
        this->content_type_ = content_type;
    }
}


part_value_t::part_value_t(std::string&& path, std::string&& content_type):
    filename(std::forward<std::string>(filename))
{
    if (content_type.empty()) {
        this->content_type_ = detect_content_type(this->filename);
    } else {
        this->content_type_ = content_type;
    }
}


std::string part_value_t::basename() const
{
    return PYCPP_NAMESPACE::base_name(filename);
}


std::string part_value_t::name() const
{
    return splitext(basename())[0];
}


const std::string & part_value_t::content_type() const
{
    return content_type_;
}


/**
 *  RFC-7231 clearly states, if the content type is unknown, do not
 *  send it.
 */
std::string part_value_t::string() const
{
    std::ostringstream stream;
    stream << "Content-Disposition: form-data; "
           << "name=\"" <<  name() << "\"; "
           << "filename=\"" << basename() << "\"\r\n";

    if (!content_type().empty()) {
       stream << "Content-Type: " << content_type() << "\r\n";
    }
    stream << "\r\n";

    return stream.str();
}


std::string file_value_t::buffer() const
{
#if defined(OS_WINDOWS)
    // WIN32 has the wide API for files, using UTF-16
    return read_wide(filename);
#else
    return read_narrow(filename);
#endif
}


std::string file_value_t::string() const
{
    std::ostringstream stream;
    stream << part_value_t::string() << buffer() << "\r\n";

    return stream.str();
}


buffer_value_t::buffer_value_t(const std::string &filename,
        const std::string &buffer,
        const std::string &content_type):
    part_value_t(filename, content_type),
    buffer_(buffer)
{}


buffer_value_t::buffer_value_t(std::string &&filename,
        std::string &&buffer,
        std::string &&content_type):
    part_value_t(std::forward<std::string>(filename), std::forward<std::string>(content_type)),
    buffer_(std::forward<std::string>(buffer))
{}


const std::string & buffer_value_t::buffer() const
{
    return buffer_;
}


std::string buffer_value_t::string() const
{
    std::ostringstream stream;
    stream << part_value_t::string() << buffer() << "\r\n";

    return stream.str();
}

}   /* detail */


const std::string & multipart_t::boundary() const
{
    return boundary_;
}


void multipart_t::add(const detail::part_ptr_t &part)
{
    push_back(part);
}


void multipart_t::add(detail::part_ptr_t&& part)
{
    emplace_back(std::forward<detail::part_ptr_t>(part));
}


std::string multipart_t::string() const
{
    std::ostringstream stream;
    for (const auto &item: *this) {
        stream << "--" << boundary() << "\r\n" << item->string();
    }

    // if any elements were written, write a trailing separator.
    if (*this) {
        stream << "--" << boundary() << "--\r\n";
    }

    return stream.str();
}


std::string multipart_t::header() const
{
    return "multipart/form-data; boundary=" + boundary();
}


multipart_t::operator bool() const
{
    return !empty();
}

PYCPP_END_NAMESPACE
