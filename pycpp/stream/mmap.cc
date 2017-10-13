//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/filesystem/exception.h>
#include <pycpp/preprocessor/architecture.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stream/mmap.h>
#include <cassert>

#if defined(HAVE_MMAP)
#   include <sys/mman.h>
#   include <pycpp/preprocessor/sysstat.h>
#elif defined(OS_WINDOWS)
#   include <pycpp/windows/winapi.h>
#endif

PYCPP_BEGIN_NAMESPACE

// mmap
// MAP_SHARED
// PROT_READ
// PROT_WRITE
// munmap

// HELPERS
// -------


#if defined(OS_POSIX)                   // POSIX

static size_t file_length(fd_t fd)
{
    struct stat sb;
    if (::fstat(fd, &sb) == -1) {
        throw filesystem_error(filesystem_unexpected_error);
    }
    return sb.st_size;
}

#elif defined(OS_WINDOWS)               // WINDOWS

static size_t file_length(fd_t fd)
{
    LARGE_INTEGER bytes;
    if (!::GetFileSizeEx(fd, &bytes)) {
        throw filesystem_error(filesystem_unexpected_error);
    }
    return static_cast<size_t>(bytes.QuadPart);
}

#else                                   // UNKNOWN
#   error "Unsupported operating system."
#endif                                  // POSIX


static int convert_prot(std::ios_base::openmode mode)
{
#if defined(HAVE_MMAP)      // MMAP

    int prot = 0;
    if (mode & std::ios_base::in) {
        prot |= PROT_READ;
    }
    if (mode & std::ios_base::out) {
        prot |= PROT_WRITE;
    }

    return prot;

#elif defined(OS_WINDOWS)

    if (mode & std::ios_base::out) {
        return PAGE_READWRITE;
    } else {
        return PAGE_READONLY;
    }

#else
#   error "System does not define mmap() or a suitable alternative.";
#endif                      // MMAP
}


#if SYSTEM_ARCHITECTURE >= 64       // >= 64-bit size_t

static uint32_t lower_byte_size(size_t bytes)
{
    static constexpr uint32_t mask = 0xFFFFFFFFUL;
    return static_cast<uint32_t>(bytes & mask);
}


static uint32_t higher_byte_size(size_t bytes)
{
    static constexpr uint32_t mask = 0xFFFFFFFFUL;
    return static_cast<uint32_t>((bytes >> 32) & mask);
}

#else                               // <= 32-bit size_t

static uint32_t lower_byte_size(size_t bytes)
{
    // sizeof(size_t) is <= sizeof(uint32_t), same value
    return bytes;
}


static uint32_t higher_byte_size(size_t bytes)
{
    // sizeof(size_t) is <= sizeof(uint32_t), no upper bytes
    return 0;
}

#endif                              // >= 64-bit size_t



#if defined(OS_WINDOWS)


static DWORD get_system_granularity()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwAllocationGranularity;
}


static DWORD convert_access(std::ios_base::openmode mode)
{
    if (mode & std::ios_base::out) {
        // provides read-write access, since Windows
        // does not have "write-only" modes.
        return FILE_MAP_WRITE;
    } else {
        return FILE_MAP_READ;
    }
}


static HANDLE create_memory_mapping(fd_t fd, std::ios_base::openmode mode, size_t offset, size_t length)
{
    static DWORD granularity = get_system_granularity();
    size_t map_size = offset + length;
    DWORD low = lower_byte_size(map_size);
    DWORD high = higher_byte_size(map_size);

    return CreateFileMapping(fd, nullptr, convert_prot(mode), high, low, NULL);
}


static void close_memory_mapping(fd_t fd)
{
    CloseHandle(fd);
}

#endif


static void* open_memory_view(fd_t fd, std::ios_base::openmode mode, size_t offset, size_t length)
{
    if (length == 0) {
        // cannot map an empty region
        return nullptr;
    }

#if defined(HAVE_MMAP)
    return ::mmap(nullptr, length, convert_prot(mode), MAP_SHARED, fd, offset);
#elif defined(OS_WINDOWS)
    static DWORD granularity = get_system_granularity();
    DWORD access = convert_access(mode);
    size_t start = (offset % granularity) * granularity;
    DWORD view_size = (offset % granularity) + length;
    DWORD low = lower_byte_size(start);
    DWORD high = higher_byte_size(start);
    return ::MapViewOfFile(fd, access, high, low, view_size);
#else
#   error "System does not define mmap() or a suitable alternative.";
#endif
}


static int memory_sync(void *addr, size_t length, bool async)
{
#if defined(HAVE_MMAP)
    // on modern Linux, MS_ASYNC is a no-op, however,
    // it still should be used for futureproofing
    int flags = async ? MS_ASYNC : MS_SYNC;
    return ::msync(addr, length, flags);
#elif defined(OS_WINDOWS)
    return ::FlushViewOfFile(addr, length) != 0;
#else
#   error "System does not define mmap() or a suitable alternative.";
#endif
}


static int close_memory_view(void *addr, size_t length)
{
#if defined(HAVE_MMAP)
    return ::munmap(addr, length);
#elif defined(OS_WINDOWS)
    return ::UnmapViewOfFile(addr) != 0;
#else
#   error "System does not define mmap() or a suitable alternative.";
#endif
}

// OBJECTS
// -------

// MMAP FSTREAM

mmap_fstream::mmap_fstream():
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{}


mmap_fstream::~mmap_fstream()
{
    close();
}


mmap_fstream::mmap_fstream(mmap_fstream &&other):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    swap(other);
}


mmap_fstream & mmap_fstream::operator=(mmap_fstream &&other)
{
    swap(other);
    return *this;
}


mmap_fstream::mmap_fstream(const std::string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    open(name, mode);
}


void mmap_fstream::open(const std::string& name, std::ios_base::openmode mode)
{
    buffer.fd(fd_open(name, mode, access_random));
}

#if defined(PYCPP_HAVE_WFOPEN)

mmap_fstream::mmap_fstream(const std::wstring& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    open(name, mode);
}


void mmap_fstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


mmap_fstream::mmap_fstream(const std::u16string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in | std::ios_base::out, INVALID_FD_VALUE),
    std::iostream(&buffer)
{
    open(name, mode);
}


void mmap_fstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    buffer.fd(fd_open(name, mode, access_random));
}

#endif                              // PYCPP_HAVE_WFOPEN

bool mmap_fstream::is_open() const
{
    return buffer.is_open();
}


bool mmap_fstream::has_mapping() const
{
    return bool(data_);
}


void mmap_fstream::close()
{
    unmap();
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void mmap_fstream::swap(mmap_fstream &other)
{
    // swap
    std::iostream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(data_, other.data_);
    std::swap(length_, other.length_);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


char& mmap_fstream::operator[](size_t index)
{
    assert(data_ && "Memory address cannot be null.");
    assert(index < length_ && "Index must be less than buffer length.");
    return data_[index];
}


const char& mmap_fstream::operator[](size_t index) const
{
    assert(data_ && "Memory address cannot be null.");
    assert(index < length_ && "Index must be less than buffer length.");
    return data_[index];
}


char* mmap_fstream::data() const
{
    return data_;
}


size_t mmap_fstream::length() const
{
    return length_;
}


void mmap_fstream::map(size_t o)
{
    map(o, file_length(buffer.fd()));
}


void mmap_fstream::map(size_t o, size_t l)
{
    unmap();
    std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out;
#if defined(OS_WINDOWS)
    map_fd = create_memory_mapping(map_fd, mode, o, l);
    if (map_fd != INVALID_FD_VALUE) {
        data_ = reinterpret_cast<char*>(open_memory_view(map_fd, mode, o, l));
    }
#else
    data_ = reinterpret_cast<char*>(open_memory_view(buffer.fd(), mode, o, l));
#endif
    if (data_) {
        length_ = l;
    }
}


void mmap_fstream::unmap()
{
    // unmap
    if (data_) {
        close_memory_view(data_, length_);
#if defined(OS_WINDOWS)
        close_memory_mapping(map_fd);
        map_fd = INVALID_FD_VALUE;
#endif
        data_ = nullptr;
        length_ = 0;
    }
}


void mmap_fstream::flush(bool async)
{
    assert(data_ && "Memory address cannot be null.");
    memory_sync(data_, length_, async);
#if defined(OS_WINDOWS)
    if (!async) {
        FlushFileBuffers(map_fd);
    }
#endif
}

// MMAP IFSTREAM


mmap_ifstream::mmap_ifstream():
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{}


mmap_ifstream::~mmap_ifstream()
{
    close();
}


mmap_ifstream::mmap_ifstream(mmap_ifstream &&other):
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{
    swap(other);
}


mmap_ifstream & mmap_ifstream::operator=(mmap_ifstream &&other)
{
    swap(other);
    return *this;
}


mmap_ifstream::mmap_ifstream(const std::string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{
    open(name, mode);
}


void mmap_ifstream::open(const std::string& name, std::ios_base::openmode mode)
{
    buffer.fd(fd_open(name, mode, access_random));
}

#if defined(PYCPP_HAVE_WFOPEN)

mmap_ifstream::mmap_ifstream(const std::wstring& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{
    open(name, mode);
}


void mmap_ifstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


mmap_ifstream::mmap_ifstream(const std::u16string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::in, INVALID_FD_VALUE),
    std::istream(&buffer)
{
    open(name, mode);
}


void mmap_ifstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    buffer.fd(fd_open(name, mode, access_random));
}

#endif                              // PYCPP_HAVE_WFOPEN

bool mmap_ifstream::is_open() const
{
    return buffer.is_open();
}


bool mmap_ifstream::has_mapping() const
{
    return bool(data_);
}


void mmap_ifstream::close()
{
    unmap();
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void mmap_ifstream::swap(mmap_ifstream &other)
{
    // swap
    std::istream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(data_, other.data_);
    std::swap(length_, other.length_);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


const char& mmap_ifstream::operator[](size_t index) const
{
    assert(data_ && "Memory address cannot be null.");
    assert(index < length_ && "Index must be less than buffer length.");
    return data_[index];
}


const char* mmap_ifstream::data() const
{
    return data_;
}


size_t mmap_ifstream::length() const
{
    return length_;
}


void mmap_ifstream::map(size_t o)
{
    map(o, file_length(buffer.fd()));
}


void mmap_ifstream::map(size_t o, size_t l)
{
    unmap();
    std::ios_base::openmode mode = std::ios_base::in;
#if defined(OS_WINDOWS)
    map_fd = create_memory_mapping(map_fd, mode, o, l);
    if (map_fd != INVALID_FD_VALUE) {
        data_ = reinterpret_cast<char*>(open_memory_view(map_fd, mode, o, l));
    }
#else
    data_ = reinterpret_cast<char*>(open_memory_view(buffer.fd(), mode, o, l));
#endif
    if (data_) {
        length_ = l;
    }
}


void mmap_ifstream::unmap()
{
    // unmap
    if (data_) {
        close_memory_view(data_, length_);
#if defined(OS_WINDOWS)
        close_memory_mapping(map_fd);
        map_fd = INVALID_FD_VALUE;
#endif
        data_ = nullptr;
        length_ = 0;
    }
}


void mmap_ifstream::flush(bool async)
{
    assert(data_ && "Memory address cannot be null.");
    memory_sync(data_, length_, async);
#if defined(OS_WINDOWS)
    if (!async) {
        FlushFileBuffers(map_fd);
    }
#endif
}

// MMAP OFSTREAM

mmap_ofstream::mmap_ofstream():
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{}


mmap_ofstream::~mmap_ofstream()
{
    close();
}


mmap_ofstream::mmap_ofstream(mmap_ofstream &&other):
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{
    swap(other);
}


mmap_ofstream & mmap_ofstream::operator=(mmap_ofstream &&other)
{
    swap(other);
    return *this;
}


mmap_ofstream::mmap_ofstream(const std::string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{
    open(name, mode);
}


void mmap_ofstream::open(const std::string& name, std::ios_base::openmode mode)
{
    buffer.fd(fd_open(name, mode, access_random));
}

#if defined(PYCPP_HAVE_WFOPEN)

mmap_ofstream::mmap_ofstream(const std::wstring& name, std::ios_base::openmode mode):
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{
    open(name, mode);
}


void mmap_ofstream::open(const std::wstring& name, std::ios_base::openmode mode)
{
    open(reinterpret_cast<const char16_t*>(name.data()), mode);
}


mmap_ofstream::mmap_ofstream(const std::u16string& name, std::ios_base::openmode mode):
    buffer(std::ios_base::out, INVALID_FD_VALUE),
    std::ostream(&buffer)
{
    open(name, mode);
}


void mmap_ofstream::open(const std::u16string& name, std::ios_base::openmode mode)
{
    buffer.fd(fd_open(name, mode, access_random));
}

#endif                              // PYCPP_HAVE_WFOPEN

bool mmap_ofstream::is_open() const
{
    return buffer.is_open();
}


bool mmap_ofstream::has_mapping() const
{
    return bool(data_);
}


void mmap_ofstream::close()
{
    unmap();
    if (buffer.fd() != INVALID_FD_VALUE) {
        buffer.close();
        fd_close(buffer.fd());
        buffer.fd(INVALID_FD_VALUE);
    }
}


void mmap_ofstream::swap(mmap_ofstream &other)
{
    // swap
    std::ostream::swap(other);
    std::swap(buffer, other.buffer);
    std::swap(data_, other.data_);
    std::swap(length_, other.length_);

    // set filebuffers
    std::ios::rdbuf(&buffer);
    other.rdbuf(&other.buffer);
}


char& mmap_ofstream::operator[](size_t index)
{
    assert(data_ && "Memory address cannot be null.");
    assert(index < length_ && "Index must be less than buffer length.");
    return data_[index];
}


char* mmap_ofstream::data() const
{
    return data_;
}


size_t mmap_ofstream::length() const
{
    return length_;
}


void mmap_ofstream::map(size_t o)
{
    map(o, file_length(buffer.fd()));
}


void mmap_ofstream::map(size_t o, size_t l)
{
    unmap();
    std::ios_base::openmode mode = std::ios_base::out;
#if defined(OS_WINDOWS)
    map_fd = create_memory_mapping(map_fd, mode, o, l);
    if (map_fd != INVALID_FD_VALUE) {
        data_ = reinterpret_cast<char*>(open_memory_view(map_fd, mode, o, l));
    }
#else
    data_ = reinterpret_cast<char*>(open_memory_view(buffer.fd(), mode, o, l));
#endif
    if (data_) {
        length_ = l;
    }
}


void mmap_ofstream::unmap()
{
    // unmap
    if (data_) {
        close_memory_view(data_, length_);
#if defined(OS_WINDOWS)
        close_memory_mapping(map_fd);
        map_fd = INVALID_FD_VALUE;
#endif
        data_ = nullptr;
        length_ = 0;
    }
}


void mmap_ofstream::flush(bool async)
{
    assert(data_ && "Memory address cannot be null.");
    memory_sync(data_, length_, async);
#if defined(OS_WINDOWS)
    if (!async) {
        FlushFileBuffers(map_fd);
    }
#endif
}

PYCPP_END_NAMESPACE
