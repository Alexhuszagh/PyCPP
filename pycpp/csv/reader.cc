//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/csv/reader.h>
#include <pycpp/string/getline.h>

#include <cassert>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

static std::string readline(std::istream& stream)
{
    std::string line;
    getline(stream, line);
    return line;
}

static csv_row parse_csv_row(std::istream& stream, csvpunct_impl& punct, size_t size)
{
    csv_row row;
    std::string line = readline(stream);
    row.reserve(size);

    bool quote = false;
    bool escape = false;
    std::vector<char> word(line.size());

    int j = 0;
    int k = 0;
    for (const char c : line) {
        if (escape) {               // escape letter and undo escaping
            escape = false;
            word[j] = c;
            j++;
        }  else if (c == punct.escape()) {
            escape = true;          // escape next character
        } else if (c == punct.quote()) {
            quote ^= true;          // opening/ending quote
        } else if (quote) {
            word[j] = c;            // append quoted character to word
            j++;
        } else if (c == punct.delimiter()) {
            row.emplace_back(std::string(word.data(), j));
            j = 0;
            k++;
        } else {
            word[j] = c;            // append unquoted word
            j++;
        }
    }

    row.emplace_back(std::string(word.data(), j));

    return row;
}

// OBJECTS
// -------

csv_stream_reader::csv_stream_reader():
    punct_(new csvpunct)
{}


csv_stream_reader::csv_stream_reader(std::istream& stream, size_t skip):
    punct_(new csvpunct)
{
    parse(stream, skip);
}


void csv_stream_reader::parse(std::istream& stream, size_t skip)
{
    // skip lines
    std::string line;
    while (skip--) {
        getline(stream, line);
    }
    stream_ = &stream;
}


void csv_stream_reader::punctuation(csvpunct_impl* punct)
{
    punct_.reset(punct ? punct : new csvpunct);
}


const csvpunct_impl* csv_stream_reader::punctuation() const
{
    return punct_.get();
}


auto csv_stream_reader::operator()() -> value_type
{
    assert(stream_ && "Stream cannot be null.");

    value_type row;
    row = parse_csv_row(*stream_, *punct_, row_length_);
    row_length_ = row.size();
    return row;
}


bool csv_stream_reader::eof() const
{
    assert(stream_ && "Stream cannot be null.");
    return (stream_->eof() || stream_->peek() == EOF || stream_->peek() == '\0');
}


csv_stream_reader::operator bool() const
{
    assert(stream_ && "Stream cannot be null.");
    return !(stream_->bad() || eof());
}


auto csv_stream_reader::begin() -> iterator
{
    return iterator(*this);
}


auto csv_stream_reader::end() -> iterator
{
    return iterator();
}


csv_file_reader::csv_file_reader()
{}


csv_file_reader::csv_file_reader(const std::string &name)
{
    parse(name, 0);
}


void csv_file_reader::open(const std::string &name)
{
    file_.open(name, std::ios_base::in | std::ios_base::binary);
}


void csv_file_reader::parse(const std::string &name, size_t skip)
{
    open(name);
    parse(skip);
}


#if defined(PYCPP_HAVE_WFOPEN)


csv_file_reader::csv_file_reader(const std::wstring &name)
{
    parse(name, 0);
}


void csv_file_reader::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::in | std::ios_base::binary);
}


void csv_file_reader::parse(const std::wstring &name, size_t skip)
{
    open(name);
    parse(skip);
}

#endif


void csv_file_reader::parse(size_t skip)
{
    if (!file_.is_open()) {
        throw std::runtime_error("Must open file handle prior to parsing.");
    }
    csv_stream_reader::parse(file_, skip);
}


csv_string_reader::csv_string_reader()
{}


csv_string_reader::csv_string_reader(const std::string &str)
{
    open(str);
    parse(0);
}


void csv_string_reader::open(const std::string &str)
{
    sstream_ = std::istringstream(str, std::ios_base::in | std::ios_base::binary);
    parse(0);
}


void csv_string_reader::parse(const std::string &str, size_t skip)
{
    open(str);
    parse(skip);
}


void csv_string_reader::parse(size_t skip)
{
    csv_stream_reader::parse(sstream_, skip);
}

PYCPP_END_NAMESPACE
