//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/compression/detect.h>
#include <pycpp/preprocessor/parallel.h>
#include <pycpp/stream/fstream.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


static bool detect_header(const string_wrapper& header, const magic_bytes& magic)
{
    if (header.size() < magic.front().size()) {
        return false;
    }

    return any_of(PARALLEL_EXECUTION magic.begin(), magic.end(), [&header](const string_wrapper& bytes) {
        return header.size() >= bytes.size() && equal(bytes.begin(), bytes.end(), header.begin());
    });
}


static bool detect_stream(istream& stream, const magic_bytes& magic)
{
    // all magic bytes **must** have the same length
    size_t size = magic.front().size();
    char* str = new char[size];
    stream.read(str, size);
    bool status = detect_header(string_wrapper(str, size), magic);
    delete[] str;

    return status;
}


static bool detect_path(const string_view& path, const magic_bytes& magic)
{
    ifstream stream(path, ios_base::in | ios_base::binary);
    return detect_stream(stream, magic);
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

static bool detect_path(const wstring_view& path, const magic_bytes& magic)
{
    ifstream stream(path, ios_base::in | ios_base::binary);
    return detect_stream(stream, magic);
}


static bool detect_path(const u16string_view& path, const magic_bytes& magic)
{
    ifstream stream(path, ios_base::in | ios_base::binary);
    return detect_stream(stream, magic);
}

#endif                                      // WINDOWS


// OBJECTS
// -------

// BZ2

const magic_bytes& is_bz2::magic()
{
    static string header("\x42\x5a\x68", 3);
    static vector<string_wrapper> vector = {string_wrapper(header)};
    static magic_bytes view(vector);
    return view;
}


bool is_bz2::header(const string_wrapper& header)
{
    return detect_header(header, magic());
}


bool is_bz2::stream(istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_bz2::path(const string_view& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_bz2::path(const wstring_view& path)
{
    return detect_path(path, magic());
}


bool is_bz2::path(const u16string_view& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

// ZLIB

const magic_bytes& is_zlib::magic()
{
    static string level1("\x78\x01", 2);
    static string level2("\x78\x5E", 2);
    static string level6("\x78\x9C", 2);
    static string level7("\x78\xDA", 2);
    static vector<string_wrapper> vector = {
        string_wrapper(level1),
        string_wrapper(level2),
        string_wrapper(level6),
        string_wrapper(level7),
    };
    static magic_bytes view(vector);
    return view;
}


bool is_zlib::header(const string_wrapper& header)
{
    return detect_header(header, magic());
}


bool is_zlib::stream(istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_zlib::path(const string_view& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_zlib::path(const wstring_view& path)
{
    return detect_path(path, magic());
}


bool is_zlib::path(const u16string_view& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

// GZIP

const magic_bytes& is_gzip::magic()
{
    static string header("\x1f\x8b\x08", 3);
    static vector<string_wrapper> vector = {string_wrapper(header)};
    static magic_bytes view(vector);
    return view;
}


bool is_gzip::header(const string_wrapper& header)
{
    return detect_header(header, magic());
}


bool is_gzip::stream(istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_gzip::path(const string_view& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_gzip::path(const wstring_view& path)
{
    return detect_path(path, magic());
}


bool is_gzip::path(const u16string_view& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

// LZMA

const magic_bytes& is_lzma::magic()
{
    static string header("\xFD\x37\x7A\x58\x5A\x00", 6);
    static vector<string_wrapper> vector = {string_wrapper(header)};
    static magic_bytes view(vector);
    return view;
}


bool is_lzma::header(const string_wrapper& header)
{
    return detect_header(header, magic());
}


bool is_lzma::stream(istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_lzma::path(const string_view& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_lzma::path(const wstring_view& path)
{
    return detect_path(path, magic());
}


bool is_lzma::path(const u16string_view& path)
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

    static string version1("\x01\x01", 2);
    static string version2("\x02\x01", 2);
    static vector<string_wrapper> vector = {
        string_wrapper(version1),
        string_wrapper(version2),
    };
    static magic_bytes view(vector);
    return view;
}


bool is_blosc::header(const string_wrapper& header)
{
    return detect_header(header, magic());
}


bool is_blosc::stream(istream& stream)
{
    return detect_stream(stream, magic());
}


bool is_blosc::path(const string_view& path)
{
    return detect_path(path, magic());
}


#if defined(HAVE_WFOPEN)                    // WINDOWS

bool is_blosc::path(const wstring_view& path)
{
    return detect_path(path, magic());
}


bool is_blosc::path(const u16string_view& path)
{
    return detect_path(path, magic());
}

#endif                                      // WINDOWS

PYCPP_END_NAMESPACE
