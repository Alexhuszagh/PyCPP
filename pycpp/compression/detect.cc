//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/detect.h>
#include <pycpp/preprocessor/parallel.h>
#include <pycpp/stream/fstream.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


static bool detect_header(const string_view& header, const magic_bytes& magic)
{
    if (header.size() < magic.front().size()) {
        return false;
    }

    return std::any_of(PARALLEL_EXECUTION magic.begin(), magic.end(), [&header](const string_view& bytes) {
        return std::equal(bytes.begin(), bytes.end(), header.begin());
    });
}


static bool detect_stream(std::istream& stream, const magic_bytes& magic)
{
    // all magic bytes **must** have the same length
    size_t size = magic.front().size();
    char* str = new char[size];
    stream.read(str, size);
    bool status = detect_header(string_view(str, size), magic);
    delete[] str;

    return status;
}


static bool detect_path(const std::string& path, const magic_bytes& magic)
{
    ifstream stream(path, std::ios_base::in | std::ios_base::binary);
    return detect_stream(stream, magic);
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

static bool detect_path(const std::wstring& path, const magic_bytes& magic)
{
    ifstream stream(path, std::ios_base::in | std::ios_base::binary);
    return detect_stream(stream, magic);
}


static bool detect_path(const std::u16string& path, const magic_bytes& magic)
{
    ifstream stream(path, std::ios_base::in | std::ios_base::binary);
    return detect_stream(stream, magic);
}

#endif                                      // WINDOWS


// OBJECTS
// -------

// BZ2

const magic_bytes& is_bz2::magic()
{
    static std::string header("\x42\x5a\x68", 3);
    static std::vector<string_view> vector = {string_view(header)};
    static magic_bytes view(vector);
    return view;
}


bool is_bz2::header(const string_view& header)
{
    return detect_header(header, magic());
}


bool is_bz2::stream(std::istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_bz2::path(const std::string& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_bz2::path(const std::wstring& path)
{
    return detect_path(path, magic());
}


bool is_bz2::path(const std::u16string& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

// ZLIB

const magic_bytes& is_zlib::magic()
{
    static std::string level1("\x78\x01", 2);
    static std::string level2("\x78\x5E", 2);
    static std::string level6("\x78\x9C", 2);
    static std::string level7("\x78\xDA", 2);
    static std::vector<string_view> vector = {
        string_view(level1),
        string_view(level2),
        string_view(level6),
        string_view(level7),
    };
    static magic_bytes view(vector);
    return view;
}


bool is_zlib::header(const string_view& header)
{
    return detect_header(header, magic());
}


bool is_zlib::stream(std::istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_zlib::path(const std::string& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_zlib::path(const std::wstring& path)
{
    return detect_path(path, magic());
}


bool is_zlib::path(const std::u16string& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

// GZIP

const magic_bytes& is_gzip::magic()
{
    static std::string header("\x1f\x8b\x08", 3);
    static std::vector<string_view> vector = {string_view(header)};
    static magic_bytes view(vector);
    return view;
}


bool is_gzip::header(const string_view& header)
{
    return detect_header(header, magic());
}


bool is_gzip::stream(std::istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_gzip::path(const std::string& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_gzip::path(const std::wstring& path)
{
    return detect_path(path, magic());
}


bool is_gzip::path(const std::u16string& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

// LZMA

const magic_bytes& is_lzma::magic()
{
    static std::string header("\xFD\x37\x7A\x58\x5A\x00", 6);
    static std::vector<string_view> vector = {string_view(header)};
    static magic_bytes view(vector);
    return view;
}


bool is_lzma::header(const string_view& header)
{
    return detect_header(header, magic());
}


bool is_lzma::stream(std::istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_lzma::path(const std::string& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_lzma::path(const std::wstring& path)
{
    return detect_path(path, magic());
}


bool is_lzma::path(const std::u16string& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

// BLOSC

const magic_bytes& is_blosc::magic()
{
    // WARNING
    // This is a bit messy, since BLOSC doesn't have a true concept
    // of a "magic" number. However, it contains the first 2 bytes
    // for detection of the algorithm and BLOSC version for stability.
    // The first byte will be {'\x01', '\x02'}, for the BLOSC_VERSION_FORMAT.
    // The next byte is currently always '\x01', for the
    // BLOSC_${ALGORITHM}_VERSION_FORMAT.

    static std::string version1("\x01\x01", 2);
    static std::string version2("\x02\x01", 2);
    static std::vector<string_view> vector = {
        string_view(version1),
        string_view(version2),
    };
    static magic_bytes view(vector);
    return view;
}


bool is_blosc::header(const string_view& header)
{
    return detect_header(header, magic());
}


bool is_blosc::stream(std::istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_blosc::path(const std::string& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_blosc::path(const std::wstring& path)
{
    return detect_path(path, magic());
}


bool is_blosc::path(const std::u16string& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

PYCPP_END_NAMESPACE
