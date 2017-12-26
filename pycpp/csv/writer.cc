//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/csv/writer.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/string/string.h>
#include <assert.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


static bool is_minimal(const string_view& value, csvpunct_impl& punct)
{
    auto has_delimiter = value.find(punct.delimiter()) != string_view::npos;
    auto has_quote = value.find(punct.quote()) != string_view::npos;
    auto has_newline = value.find(NEWLINE) != string_view::npos;

    return !(has_delimiter || has_quote || has_newline);
}


static string quote_all(const string_view& value, csvpunct_impl& punct)
{
    // initialize parameters
    string quoted, sub, repl;
    sub += punct.quote();
    repl += punct.escape();
    repl += punct.quote();

    // quote the value
    quoted += punct.quote();
    quoted += string_wrapper(value).replace(sub, repl);
    quoted += punct.quote();

    return quoted;
}


static string quote_none(const string_view& value, csvpunct_impl& punct)
{
    return string(value);
}


static string quote_minimal(const string_view& value, csvpunct_impl& punct)
{
    if (is_minimal(value, punct)) {
        return quote_none(value, punct);
    } else {
        return quote_all(value, punct);
    }
}


static string quote_value(const string_view& value, csvpunct_impl& punct, csv_quoting quoting)
{
    switch (quoting) {
        case CSV_QUOTE_ALL:
            return quote_all(value, punct);
        case CSV_QUOTE_MINIMAL:
            return quote_minimal(value, punct);
        case CSV_QUOTE_NONE:
            return quote_none(value, punct);
        default:
            printf("Unrecognized CSV quoting option.");
            abort();
    }
}

// OBJECTS
// -------

csv_stream_writer::csv_stream_writer(csv_quoting quoting, csvpunct_impl* punct):
    quoting_(quoting),
    punct_(punct ? punct : new csvpunct)
{}


csv_stream_writer::csv_stream_writer(csv_stream_writer&& rhs):
    csv_stream_writer()
{
    swap(rhs);
}


csv_stream_writer& csv_stream_writer::operator=(csv_stream_writer&& rhs)
{
    swap(rhs);
    return *this;
}


csv_stream_writer::csv_stream_writer(ostream& stream, csv_quoting quoting, csvpunct_impl* punct):
    punct_(punct ? punct : new csvpunct)
{
    open(stream, quoting, nullptr);
}


void csv_stream_writer::open(ostream& stream, csv_quoting q, csvpunct_impl* p)
{
    stream_ = &stream;
    if (p) {
        punctuation(p);
    }
    quoting(q);
}


void csv_stream_writer::punctuation(csvpunct_impl* punct)
{
    punct_.reset(punct ? punct : new csvpunct);
}


const csvpunct_impl* csv_stream_writer::punctuation() const
{
    return punct_.get();
}


void csv_stream_writer::quoting(csv_quoting q)
{
    quoting_ = q;
}


const csv_quoting csv_stream_writer::quoting() const
{
    return quoting_;
}


void csv_stream_writer::swap(csv_stream_writer& rhs)
{
    using PYCPP_NAMESPACE::swap;

    swap(stream_, rhs.stream_);
    swap(quoting_, rhs.quoting_);
    swap(punct_, rhs.punct_);
}


void csv_stream_writer::operator()(const value_type& row)
{
    string output;
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


csv_file_writer::csv_file_writer(csv_quoting quoting, csvpunct_impl* punct):
    csv_stream_writer(quoting, punct)
{}


csv_file_writer::csv_file_writer(csv_file_writer&& rhs):
    csv_file_writer()
{
    swap(rhs);
}


csv_file_writer& csv_file_writer::operator=(csv_file_writer&& rhs)
{
    swap(rhs);
    return *this;
}


csv_file_writer::csv_file_writer(const string_view& name, csv_quoting quoting, csvpunct_impl* punct)
{
    open(name, quoting, punct);
}


void csv_file_writer::open(const string_view& name, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, ios_base::out | ios_base::binary);
    csv_stream_writer::open(file_, quoting, punct);
}


#if defined(HAVE_WFOPEN)                        // WINDOWS

csv_file_writer::csv_file_writer(const wstring_view& name, csv_quoting quoting, csvpunct_impl* punct):
    csv_stream_writer(quoting)
{
    open(name);
}


void csv_file_writer::open(const wstring_view& name, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, ios_base::out | ios_base::binary);
    csv_stream_writer::open(file_, quoting, punct);
}


csv_file_writer::csv_file_writer(const u16string_view& name, csv_quoting quoting, csvpunct_impl* punct):
    csv_stream_writer(quoting)
{
    open(name);
}


void csv_file_writer::open(const u16string_view& name, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, ios_base::out | ios_base::binary);
    csv_stream_writer::open(file_, quoting, punct);
}

#endif                                          // WINDOWS


void csv_file_writer::swap(csv_file_writer& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(file_, rhs.file_);
    swap(quoting_, rhs.quoting_);
    swap(punct_, rhs.punct_);

    // swap the streams, considering null conditions
    if (stream_ && rhs.stream_) {
        // do nothing
    } else if (stream_) {
        stream_ = nullptr;
        rhs.stream_ = &rhs.file_;
    } else if (rhs.stream_) {
        rhs.stream_ = nullptr;
        stream_ = &file_;
    }
}


csv_string_writer::csv_string_writer(csv_quoting quoting, csvpunct_impl* punct):
    sstream_(ios_base::out | ios_base::binary)
{
    csv_stream_writer::open(sstream_, quoting, punct);
}


csv_string_writer::csv_string_writer(csv_string_writer&& rhs):
    csv_string_writer()
{
    swap(rhs);
}


csv_string_writer& csv_string_writer::operator=(csv_string_writer&& rhs)
{
    swap(rhs);
    return *this;
}


string csv_string_writer::str() const
{
    return sstream_.str();
}


void csv_string_writer::swap(csv_string_writer& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(sstream_, rhs.sstream_);
    swap(quoting_, rhs.quoting_);
    swap(punct_, rhs.punct_);

    // swap the streams, considering null conditions
    if (stream_ && rhs.stream_) {
        // do nothing
    } else if (stream_) {
        stream_ = nullptr;
        rhs.stream_ = &rhs.sstream_;
    } else if (rhs.stream_) {
        rhs.stream_ = nullptr;
        stream_ = &sstream_;
    }
}

PYCPP_END_NAMESPACE
