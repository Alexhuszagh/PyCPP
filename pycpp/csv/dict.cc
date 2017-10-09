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

csv_dict_stream_reader::csv_dict_stream_reader()
{}


csv_dict_stream_reader::csv_dict_stream_reader(std::istream& stream, size_t skip)
{
    parse(stream, skip);
}


void csv_dict_stream_reader::parse(std::istream& stream, size_t skip)
{
    reader_.parse(stream, skip);
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


csv_dict_file_reader::csv_dict_file_reader()
{}


csv_dict_file_reader::csv_dict_file_reader(const std::string &name)
{
    parse(name, 0);
}


void csv_dict_file_reader::open(const std::string &name)
{
    file_.open(name, std::ios_base::in | std::ios_base::binary);
}


void csv_dict_file_reader::parse(const std::string &name, size_t skip)
{
    open(name);
    parse(skip);
}


#if defined(PYCPP_HAVE_WFOPEN)


csv_dict_file_reader::csv_dict_file_reader(const std::wstring &name)
{
    parse(name, 0);
}


void csv_dict_file_reader::open(const std::wstring &name)
{
    file_.open(name, std::ios_base::in | std::ios_base::binary);
}


void csv_dict_file_reader::parse(const std::wstring &name, size_t skip)
{
    open(name);
    parse(skip);
}

#endif


void csv_dict_file_reader::parse(size_t skip)
{
    if (!file_.is_open()) {
        throw std::runtime_error("Must open file handle prior to parsing.");
    }
    csv_dict_stream_reader::parse(file_, skip);
}


csv_dict_string_reader::csv_dict_string_reader()
{}


csv_dict_string_reader::csv_dict_string_reader(const std::string &str)
{
    open(str);
    parse(0);
}


void csv_dict_string_reader::open(const std::string &str)
{
    sstream_ = std::istringstream(str, std::ios_base::binary);
    parse(0);
}


void csv_dict_string_reader::parse(const std::string &str, size_t skip)
{
    open(str);
    parse(skip);
}


void csv_dict_string_reader::parse(size_t skip)
{
    csv_dict_stream_reader::parse(sstream_, skip);
}


csv_dict_stream_writer::csv_dict_stream_writer(csv_quoting quoting):
    writer_(quoting)
{}


csv_dict_stream_writer::csv_dict_stream_writer(std::ostream& stream, const csv_row& header, csv_quoting quoting):
    writer_(stream, quoting)
{
    header_ = parse_header(header);
}


void csv_dict_stream_writer::open(std::ostream& stream, const csv_row& header)
{
    writer_.open(stream);
    header_ = parse_header(header);
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


csv_dict_file_writer::csv_dict_file_writer()
{}


csv_dict_file_writer::csv_dict_file_writer(const std::string &name, const csv_row& header)
{
    open(name, header);
}


void csv_dict_file_writer::open(const std::string &name, const csv_row& header)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    csv_dict_stream_writer::open(file_, header);
}

#if defined(PYCPP_HAVE_WFOPEN)

csv_dict_file_writer::csv_dict_file_writer(const std::wstring &name, const csv_row& header)
{
    open(name, header);
}


void csv_dict_file_writer::open(const std::wstring &name, const csv_row& header)
{
    file_.open(name, std::ios_base::out | std::ios_base::binary);
    csv_dict_stream_writer::open(file_, header);
}

#endif


csv_dict_string_writer::csv_dict_string_writer()
{}


csv_dict_string_writer::csv_dict_string_writer(const std::string &str, const csv_row& header)
{
    open(str, header);
}


void csv_dict_string_writer::open(const std::string &str, const csv_row& header)
{
    sstream_ = std::ostringstream(str, std::ios_base::out | std::ios_base::binary);
    csv_dict_stream_writer::open(sstream_, header);
}


PYCPP_END_NAMESPACE
