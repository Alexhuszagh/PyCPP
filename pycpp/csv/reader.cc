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

static csv_row parse_csv_row(std::istream& stream, csvpunct& punct, size_t size)
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
}

// OBJECTS
// -------

csv_stream_reader::csv_stream_reader(const csvpunct& punct):
    punct_(punct)
{}


csv_stream_reader::csv_stream_reader(csvpunct&& punct):
    punct_(std::move(punct))
{}


csv_stream_reader::csv_stream_reader(const csvpunct& punct, std::istream& stream, size_t skip):
    punct_(punct)
{
    parse(stream, skip);
}


csv_stream_reader::csv_stream_reader(csvpunct&& punct, std::istream& stream, size_t skip):
    punct_(std::move(punct))
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

    // initialize reader
    stream_ = &stream;
    csv_row header = parse_csv_row(*stream_, punct_, 0);
    header_.reserve(header.size());
    for (size_t i = 0; i < header.size(); ++i) {
        header_[header[i]] = i;
    }
}


auto csv_stream_reader::operator()() -> value_type
{
    assert(stream_ && "Stream cannot be null.");
    return parse_csv_row(*stream_, punct_, header_.size());
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

PYCPP_END_NAMESPACE
