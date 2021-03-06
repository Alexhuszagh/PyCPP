//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/preprocessor/sysstat.h>
#include <pycpp/random.h>
#include <pycpp/stream/fd.h>

#if defined(OS_WINDOWS)          // WINDOWS NT
#   include <pycpp/windows/winapi.h>
#else                           // POSIX
#   include <fcntl.h>
#   include <unistd.h>
#endif

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

#if defined(OS_WINDOWS)          // WINDOWS NT

static path_t gettempdir_impl()
{
    wchar_t *dir = _wgetenv(L"TMPDIR");
    if (dir != nullptr) {
        return path_t(reinterpret_cast<char16_t*>(dir));
    }

    dir = _wgetenv(L"TEMP");
    if (dir != nullptr) {
        return path_t(reinterpret_cast<char16_t*>(dir));
    }

    dir = _wgetenv(L"TMP");
    if (dir != nullptr) {
        return path_t(reinterpret_cast<char16_t*>(dir));
    } else {
        // temp directory not defined, return root
        return path_t(reinterpret_cast<const char16_t*>(L"c:"));
    }
}


static path_t gettempprefix_impl()
{
    return path_t(reinterpret_cast<const char16_t*>(L"tmp"));
}


static fd_stream temporary_file_impl(const path_view_t& dir, const path_view_t& prefix)
{

    path_t name;
    fd_t fd;
    DWORD access = GENERIC_READ | GENERIC_WRITE;
    DWORD share = 0;
    LPSECURITY_ATTRIBUTES security = nullptr;
    DWORD create = OPEN_EXISTING;
    DWORD flags = 0;
    HANDLE file = nullptr;

    for (size_t i = 0; i < TMP_MAX_PATHS; ++i) {
        name = gettempnam(dir, prefix);
        auto path = reinterpret_cast<const wchar_t*>(name.data());
        fd = CreateFileW(path, access, share, security, create, flags, file);
        if (fd != INVALID_FD_VALUE) {
            return fd_stream(fd, true);
        }
    }

    throw runtime_error("Unable to find suitable temporary file name.");
}

#else                           // POSIX

static path_t gettempdir_impl()
{
    char *dir = getenv("TMPDIR");
    if (dir != nullptr) {
        return path_t(dir);
    }

    dir = getenv("TEMP");
    if (dir != nullptr) {
        return path_t(dir);
    }

    dir = getenv("TMP");
    if (dir != nullptr) {
        return path_t(dir);
    } else {
        // temp directory not defined, return root
        return path_t("/");
    }
}


static path_t gettempprefix_impl()
{
    return path_t("tmp");
}

static fd_stream temporary_file_impl(const path_view_t& dir, const path_view_t& prefix)
{
    path_t name;
    fd_t fd;
    int flags = flags = O_RDWR | O_CREAT | O_EXCL;
    mode_t permission = S_IRUSR | S_IWUSR;

    for (size_t i = 0; i < TMP_MAX_PATHS; ++i) {
        name = gettempnam(dir, prefix);
        fd = ::open(name.data(), flags, permission);
        if (fd != INVALID_FD_VALUE) {
            return fd_stream(fd, true);
        }
    }

    throw runtime_error("Unable to find suitable temporary file name.");
}


#endif


static path_t gettempsuffix()
{
    auto first = TMP_SUFFIX_CHARACTERS.begin();
    auto last = TMP_SUFFIX_CHARACTERS.end();

    path_t suffix;
    suffix.reserve(TMP_SUFFIX_LENGTH);
    for (size_t i = 0; i < TMP_SUFFIX_LENGTH; ++i) {
        suffix.push_back(PYCPP_NAMESPACE::choice(first, last));
    }

    return suffix;
}


// VARIABLES
// ---------

path_t tempdir = gettempdir_impl();
path_t tempprefix = gettempprefix_impl();
size_t TMP_MAX_PATHS = 100;
size_t TMP_SUFFIX_LENGTH = 15;
path_t TMP_SUFFIX_CHARACTERS = path_prefix("abcdefghijklmnopqrstuvwxyz0123456789_");

// FUNCTIONS
// ---------


fd_stream temporary_file(const path_view_t& dir, const path_view_t& prefix)
{
    return temporary_file_impl(dir, prefix);
}


path_t temporary_directory(const path_view_t& dir, const path_view_t& prefix)
{
    path_t name;
    mode_t permission = S_IRUSR | S_IWUSR;
    for (size_t i = 0; i < TMP_MAX_PATHS; ++i) {
        name = gettempnam(dir, prefix);
        if (mkdir(name, permission)) {
            return name;
        }
    }

    throw runtime_error("Unable to find suitable temporary file name.");
}


path_view_t gettempdir()
{
    return path_view_t(tempdir);
}


path_view_t gettempprefix()
{
    return path_view_t(tempprefix);
}


path_t gettempnam(const path_view_t& dir, const path_view_t& prefix)
{
    path_list_t paths;
    if (prefix.empty() && dir.empty()) {
        paths = {path_t(gettempdir()), path_t(gettempprefix()) + path_t(gettempsuffix())};
    } else if (dir.empty()) {
        paths = {path_t(gettempdir()), path_t(prefix) + path_t(gettempsuffix())};
    } else if (prefix.empty()) {
        paths = {path_t(dir), path_t(gettempprefix()) + path_t(gettempsuffix())};
    } else {
        paths = {path_t(dir), path_t(prefix) + path_t(gettempsuffix())};
    }
    return join_path(paths);
}

#if defined(OS_WINDOWS)          // WINDOWS NT && BACKUP PATH

fd_stream temporary_filew(const path_view_t& dir, const path_view_t& prefix)
{
    return temporary_file(dir, prefix);
}


path_t temporary_directoryw(const path_view_t& dir, const path_view_t& prefix)
{
    return temporary_directory(dir, prefix);
}


path_view_t gettempdirw()
{
    return gettempdir();
}


path_view_t gettempprefixw()
{
    return gettempprefix();
}


path_t gettempnamw(const path_view_t& dir, const path_view_t& prefix)
{
    return gettempnam(dir, prefix);
}


fd_stream temporary_filea(const backup_path_view_t& dir, const backup_path_view_t& prefix)
{
    return temporary_file(backup_path_to_path(dir), backup_path_to_path(prefix));
}


backup_path_t temporary_directorya(const backup_path_view_t& dir, const backup_path_view_t& prefix)
{
    return path_to_backup_path(temporary_directory(backup_path_to_path(dir), backup_path_to_path(prefix)));
}


backup_path_view_t gettempdira()
{
    return path_to_backup_path(gettempdirw());
}


backup_path_view_t gettempprefixa()
{
    return path_to_backup_path(gettempprefixw());
}


backup_path_t gettempnama(const backup_path_view_t& dir, const backup_path_view_t& prefix)
{
    return path_to_backup_path(gettempnam(backup_path_to_path(dir), backup_path_to_path(prefix)));
}

#else                           // POSIX

#endif

PYCPP_END_NAMESPACE
