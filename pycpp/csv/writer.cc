//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/csv/writer.h>
#include <pycpp/string/string.h>
#include <pycpp/string/whitespace.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


static bool is_minimal(const std::string& value, csvpunct_impl& punct)
{
    auto has_delimiter = value.find(punct.delimiter()) != std::string::npos;
    auto has_quote = value.find(punct.quote()) != std::string::npos;
    auto has_newline = value.find(NEWLINE) != std::string::npos;

    return !(has_delimiter || has_quote || has_newline);
}


static std::string quote_all(const std::string& value, csvpunct_impl& punct)
{
    // initialize parameters
    std::string quoted, sub, repl;
    sub += punct.quote();
    repl += punct.escape();
    repl += punct.quote();

    // quote the value
    quoted += punct.quote();
    quoted += replace(value, sub, repl);
    quoted += punct.quote();

    return quoted;
}


static std::string quote_none(const std::string& value, csvpunct_impl& punct)
{
    return value;
}


static std::string quote_minimal(const std::string& value, csvpunct_impl& punct)
{
    if (is_minimal(value, punct)) {
        return quote_none(value, punct);
    } else {
        return quote_all(value, punct);
    }
}


static std::string quote_value(const std::string& value, csvpunct_impl& punct, csv_quoting quoting)
{
    switch (quoting) {
        case CSV_QUOTE_ALL:
            return quote_all(value, punct);
        case CSV_QUOTE_MINIMAL:
            return quote_minimal(value, punct);
        case CSV_QUOTE_NONE:
            return quote_none(value, punct);
        default:
            throw std::invalid_argument("Unrecognized CSV quoting option.");
    }
}

// OBJECTS
// -------

csv_stream_writer::csv_stream_writer(csv_quoting quoting):
    quoting_(quoting),
    punct_(new csvpunct)
{}


csv_stream_writer::csv_stream_writer(std::ostream& stream, csv_quoting quoting):
    quoting_(quoting),
    punct_(new csvpunct)
{
    open(stream);
}


void csv_stream_writer::open(std::ostream& stream)
{
    stream_ = &stream;
}


void csv_stream_writer::punctuation(csvpunct_impl* punct)
{
    punct_.reset(punct ? punct : new csvpunct);
}


const csvpunct_impl* csv_stream_writer::punctuation() const
{
    return punct_.get();
}


void csv_stream_writer::operator()(const value_type& row)
{
    std::string output;
    for (const auto& item: row) {
        output += quote_value(item, *punct_, quoting_);
        output += punct_->delimiter();
    }

    if (row.size() > 0) {
        // extra trailing delimiter
        output.erase(output.size()-1);
    }

    output += NEWLINE;
    stream_->write(output.data(), output.size());
}


PYCPP_END_NAMESPACE
