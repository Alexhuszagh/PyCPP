//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/csv/dict.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

static csv_indexes parse_header(const csv_row& header)
{
    csv_indexes map;
    for (size_t i = 0; i < header.size(); i++) {
        map.emplace(move(header[i]), i);
    }
    return map;
}

// OBJECTS
// -------


csv_dict_stream_reader::csv_dict_stream_reader(csvpunct_impl* punct):
    reader_(punct)
{}


csv_dict_stream_reader::csv_dict_stream_reader(csv_dict_stream_reader&& rhs):
    csv_dict_stream_reader()
{
    swap(rhs);
}


csv_dict_stream_reader& csv_dict_stream_reader::operator=(csv_dict_stream_reader&& rhs)
{
    swap(rhs);
    return *this;
}


csv_dict_stream_reader::csv_dict_stream_reader(istream& stream, size_t skip, csvpunct_impl* punct)
{
    open(stream, skip, punct);
}


void csv_dict_stream_reader::open(istream& stream, size_t skip, csvpunct_impl* punct)
{
    reader_.open(stream, skip, punct);
    header_ = parse_header(reader_());
}


void csv_dict_stream_reader::punctuation(csvpunct_impl* punct)
{
    reader_.punctuation(punct);
}


const csvpunct_impl* csv_dict_stream_reader::punctuation() const
{
    return reader_.punctuation();
}


void csv_dict_stream_reader::swap(csv_dict_stream_reader& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(reader_, rhs.reader_);
    swap(header_, rhs.header_);
}


auto csv_dict_stream_reader::operator()() -> value_type
{
    value_type map;
    csv_row row = reader_();
    for (const auto& pair: header_) {
        map[pair.first] = move(row.at(pair.second));
    }

    return map;
}


bool csv_dict_stream_reader::eof() const
{
    return reader_.eof();
}


csv_dict_stream_reader::operator bool() const
{
    return bool(reader_);
}


auto csv_dict_stream_reader::begin() -> iterator
{
    return iterator(*this);
}


auto csv_dict_stream_reader::end() -> iterator
{
    return iterator();
}


csv_dict_file_reader::csv_dict_file_reader(csvpunct_impl* punct):
    csv_dict_stream_reader(punct)
{}


csv_dict_file_reader::csv_dict_file_reader(csv_dict_file_reader&& rhs):
    csv_dict_file_reader()
{
    swap(rhs);
}


csv_dict_file_reader& csv_dict_file_reader::operator=(csv_dict_file_reader&& rhs)
{
    swap(rhs);
    return *this;
}


csv_dict_file_reader::csv_dict_file_reader(const string_view& name, size_t skip, csvpunct_impl* punct)
{
    open(name, skip, punct);
}


void csv_dict_file_reader::open(const string_view& name, size_t skip, csvpunct_impl* punct)
{
    file_.open(name, ios_base::in | ios_base::binary);
    csv_dict_stream_reader::open(file_, skip, punct);
}


#if defined(HAVE_WFOPEN)                        // WINDOWS


csv_dict_file_reader::csv_dict_file_reader(const wstring_view& name, size_t skip, csvpunct_impl* punct)
{
    open(name, skip, punct);
}


void csv_dict_file_reader::open(const wstring_view& name, size_t skip, csvpunct_impl* punct)
{
    file_.open(name, ios_base::in | ios_base::binary);
    csv_dict_stream_reader::open(file_, skip, punct);
}



csv_dict_file_reader::csv_dict_file_reader(const u16string_view& name, size_t skip, csvpunct_impl* punct)
{
    open(name, skip, punct);
}


void csv_dict_file_reader::open(const u16string_view& name, size_t skip, csvpunct_impl* punct)
{
    file_.open(name, ios_base::in | ios_base::binary);
    csv_dict_stream_reader::open(file_, skip, punct);
}

#endif                                          // WINDOWS


void csv_dict_file_reader::swap(csv_dict_file_reader& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(file_, rhs.file_);
    swap(header_, rhs.header_);

    auto& l = reader_;
    auto& r = rhs.reader_;
    swap(l.row_length_, r.row_length_);
    swap(l.punct_, r.punct_);

    // swap the streams, considering null conditions
    if (l.stream_ && r.stream_) {
        // do nothing
    } else if (l.stream_) {
        l.stream_ = nullptr;
        r.stream_ = &rhs.file_;
    } else if (r.stream_) {
        r.stream_ = nullptr;
        l.stream_ = &file_;
    }
}


csv_dict_string_reader::csv_dict_string_reader(csvpunct_impl* punct):
    csv_dict_stream_reader(punct)
{}


csv_dict_string_reader::csv_dict_string_reader(csv_dict_string_reader&& rhs):
    csv_dict_string_reader()
{
    swap(rhs);
}


csv_dict_string_reader& csv_dict_string_reader::operator=(csv_dict_string_reader&& rhs)
{
    swap(rhs);
    return *this;
}


csv_dict_string_reader::csv_dict_string_reader(const string_wrapper& str, size_t skip, csvpunct_impl* punct)
{
    open(str, skip, punct);
}


void csv_dict_string_reader::open(const string_wrapper& str, size_t skip, csvpunct_impl* punct)
{
    sstream_ = istringstream(std::string(str), ios_base::binary);
    csv_dict_stream_reader::open(sstream_, skip, punct);
}


void csv_dict_string_reader::swap(csv_dict_string_reader& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(sstream_, rhs.sstream_);
    swap(header_, rhs.header_);

    auto& l = reader_;
    auto& r = rhs.reader_;
    swap(l.row_length_, r.row_length_);
    swap(l.punct_, r.punct_);

    // swap the streams, considering null conditions
    if (l.stream_ && r.stream_) {
        // do nothing
    } else if (l.stream_) {
        l.stream_ = nullptr;
        r.stream_ = &rhs.sstream_;
    } else if (r.stream_) {
        r.stream_ = nullptr;
        l.stream_ = &sstream_;
    }
}


csv_dict_stream_writer::csv_dict_stream_writer(csv_quoting quoting, csvpunct_impl* punct):
    writer_(quoting, punct)
{}


csv_dict_stream_writer::csv_dict_stream_writer(csv_dict_stream_writer&& rhs):
    csv_dict_stream_writer()
{
    swap(rhs);
}


csv_dict_stream_writer& csv_dict_stream_writer::operator=(csv_dict_stream_writer&& rhs)
{
    swap(rhs);
    return *this;
}


csv_dict_stream_writer::csv_dict_stream_writer(ostream& stream, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    open(stream, header, quoting, punct);
}


void csv_dict_stream_writer::open(ostream& stream, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    writer_.open(stream, quoting, punct);
    header_ = parse_header(header);
    writer_(header);
}


void csv_dict_stream_writer::punctuation(csvpunct_impl* p)
{
    writer_.punctuation(p);
}


const csvpunct_impl* csv_dict_stream_writer::punctuation() const
{
    return writer_.punctuation();
}


void csv_dict_stream_writer::quoting(csv_quoting q)
{
    writer_.quoting(q);
}


const csv_quoting csv_dict_stream_writer::quoting() const
{
    return writer_.quoting();
}


void csv_dict_stream_writer::swap(csv_dict_stream_writer& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(writer_, rhs.writer_);
    swap(header_, rhs.header_);
}


void csv_dict_stream_writer::operator()(const value_type& row)
{
    csv_row flat(row.size());
    for (const auto& pair: row) {
        size_t index = header_.at(pair.first);
        flat[index] = pair.second;
    }
    writer_(flat);
}


csv_dict_file_writer::csv_dict_file_writer(csv_quoting quoting, csvpunct_impl* punct):
    csv_dict_stream_writer(quoting, punct)
{}


csv_dict_file_writer::csv_dict_file_writer(csv_dict_file_writer&& rhs):
    csv_dict_file_writer()
{
    swap(rhs);
}


csv_dict_file_writer& csv_dict_file_writer::operator=(csv_dict_file_writer&& rhs)
{
    swap(rhs);
    return *this;
}


csv_dict_file_writer::csv_dict_file_writer(const string_view& name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    open(name, header, quoting, punct);
}


void csv_dict_file_writer::open(const string_view& name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, ios_base::out | ios_base::binary);
    csv_dict_stream_writer::open(file_, header, quoting, punct);
}

#if defined(HAVE_WFOPEN)                        // WINDOWS

csv_dict_file_writer::csv_dict_file_writer(const wstring_view& name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    open(name, header, quoting, punct);
}


void csv_dict_file_writer::open(const wstring_view& name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, ios_base::out | ios_base::binary);
    csv_dict_stream_writer::open(file_, header, quoting, punct);
}


csv_dict_file_writer::csv_dict_file_writer(const u16string_view& name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    open(name, header, quoting, punct);
}


void csv_dict_file_writer::open(const u16string_view& name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, ios_base::out | ios_base::binary);
    csv_dict_stream_writer::open(file_, header, quoting, punct);
}

#endif                                          // WINDOWS


void csv_dict_file_writer::swap(csv_dict_file_writer& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(file_, rhs.file_);
    swap(header_, rhs.header_);

    auto& l = writer_;
    auto& r = rhs.writer_;
    swap(l.quoting_, r.quoting_);
    swap(l.punct_, r.punct_);

    // swap the streams, considering null conditions
    if (l.stream_ && r.stream_) {
        // do nothing
    } else if (l.stream_) {
        l.stream_ = nullptr;
        r.stream_ = &rhs.file_;
    } else if (r.stream_) {
        r.stream_ = nullptr;
        l.stream_ = &file_;
    }
}


csv_dict_string_writer::csv_dict_string_writer(csv_quoting quoting, csvpunct_impl* punct):
    csv_dict_stream_writer(quoting, punct),
    sstream_(ios_base::out | ios_base::binary)
{}


csv_dict_string_writer::csv_dict_string_writer(csv_dict_string_writer&& rhs):
    csv_dict_string_writer()
{
    swap(rhs);
}


csv_dict_string_writer& csv_dict_string_writer::operator=(csv_dict_string_writer&& rhs)
{
    swap(rhs);
    return *this;
}


csv_dict_string_writer::csv_dict_string_writer(const csv_row& header, csv_quoting quoting, csvpunct_impl* punct):
    sstream_(ios_base::out | ios_base::binary)
{
    open(header, quoting, punct);
}


void csv_dict_string_writer::open(const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    csv_dict_stream_writer::open(sstream_, header,quoting, punct);
}


std::string csv_dict_string_writer::str() const
{
    return sstream_.str();
}


void csv_dict_string_writer::swap(csv_dict_string_writer& rhs)
{
    using PYCPP_NAMESPACE::swap;
    swap(sstream_, rhs.sstream_);
    swap(header_, rhs.header_);

    auto& l = writer_;
    auto& r = rhs.writer_;
    swap(l.quoting_, r.quoting_);
    swap(l.punct_, r.punct_);

    // swap the streams, considering null conditions
    if (l.stream_ && r.stream_) {
        // do nothing
    } else if (l.stream_) {
        l.stream_ = nullptr;
        r.stream_ = &rhs.sstream_;
    } else if (r.stream_) {
        r.stream_ = nullptr;
        l.stream_ = &sstream_;
    }
}

PYCPP_END_NAMESPACE
