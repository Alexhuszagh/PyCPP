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
        map.emplace(std::move(header[i]), i);
    }
    return map;
}

// OBJECTS
// -------


csv_dict_stream_reader::csv_dict_stream_reader(csvpunct_impl* punct):
    reader_(punct)
{}


csv_dict_stream_reader::csv_dict_stream_reader(std::istream& stream, size_t skip, csvpunct_impl* punct)
{
    open(stream, skip, punct);
}


void csv_dict_stream_reader::open(std::istream& stream, size_t skip, csvpunct_impl* punct)
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


auto csv_dict_stream_reader::operator()() -> value_type
{
    value_type map;
    csv_row row = reader_();
    for (const auto& pair: header_) {
        map[pair.first] = std::move(row.at(pair.second));
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


csv_dict_file_reader::csv_dict_file_reader(const std::string &name, size_t skip, csvpunct_impl* punct)
{
    open(name, skip, punct);
}


void csv_dict_file_reader::open(const std::string &name, size_t skip, csvpunct_impl* punct)
{
    file_.open(name, std::ios_base::in | std::ios_base::binary);
    csv_dict_stream_reader::open(file_, skip, punct);
}


#if defined(PYCPP_HAVE_WFOPEN)


csv_dict_file_reader::csv_dict_file_reader(const std::wstring &name, size_t skip, csvpunct_impl* punct)
{
    open(name, skip, punct);
}


void csv_dict_file_reader::open(const std::wstring &name, size_t skip, csvpunct_impl* punct)
{
    file_.open(name, std::ios_base::in | std::ios_base::binary);
    csv_dict_stream_reader::open(file_, skip, punct);
}

#endif


csv_dict_string_reader::csv_dict_string_reader(csvpunct_impl* punct):
    csv_dict_stream_reader(punct)
{}


csv_dict_string_reader::csv_dict_string_reader(const std::string &str, size_t skip, csvpunct_impl* punct)
{
    open(str, skip, punct);
}


void csv_dict_string_reader::open(const std::string &str, size_t skip, csvpunct_impl* punct)
{
    sstream_ = std::istringstream(str, std::ios_base::binary);
    csv_dict_stream_reader::open(sstream_, skip, punct);
}


csv_dict_stream_writer::csv_dict_stream_writer(csv_quoting quoting, csvpunct_impl* punct):
    writer_(quoting, punct)
{}


csv_dict_stream_writer::csv_dict_stream_writer(std::ostream& stream, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    open(stream, header, quoting, punct);
}


void csv_dict_stream_writer::open(std::ostream& stream, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
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


csv_dict_file_writer::csv_dict_file_writer(const std::string &name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    open(name, header, quoting, punct);
}


void csv_dict_file_writer::open(const std::string &name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    csv_dict_stream_writer::open(file_, header, quoting, punct);
}

#if defined(PYCPP_HAVE_WFOPEN)

csv_dict_file_writer::csv_dict_file_writer(const std::wstring &name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    open(name, header, quoting, punct);
}


void csv_dict_file_writer::open(const std::wstring &name, const csv_row& header, csv_quoting quoting, csvpunct_impl* punct)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    csv_dict_stream_writer::open(file_, header, quoting, punct);
}

#endif

csv_dict_string_writer::csv_dict_string_writer(csv_quoting quoting, csvpunct_impl* punct):
    csv_dict_stream_writer(quoting, punct),
    sstream_(std::ios_base::out | std::ios_base::binary)
{}


csv_dict_string_writer::csv_dict_string_writer(const csv_row& header, csv_quoting quoting, csvpunct_impl* punct):
    sstream_(std::ios_base::out | std::ios_base::binary)
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

PYCPP_END_NAMESPACE
