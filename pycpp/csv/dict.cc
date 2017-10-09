//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/csv/dict.h>

PYCPP_BEGIN_NAMESPACE

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

    // set the header indexes
    csv_row header = reader_();
    for (size_t i = 0; i < header.size(); i++) {
        header_.emplace(std::move(header[i]), i);
    }
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


PYCPP_END_NAMESPACE
