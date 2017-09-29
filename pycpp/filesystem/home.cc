//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------

#if defined(OS_WINDOWS)          // WINDOWS NT

static path_t gethomedir_impl()
{
    // check USERPROFILE
    wchar_t *home = _wgetenv(L"USERPROFILE");
    if (home != nullptr) {
        return path_t(reinterpret_cast<char16_t*>(home));
    }

    // check HOME
    home = _wgetenv(L"HOME");
    if (home != nullptr) {
        return path_t(reinterpret_cast<char16_t*>(home));
    }

    // combine HOMEDRIVE and HOMEPATH
    wchar_t *drive = _wgetenv(L"HOMEDRIVE");
    wchar_t *path = _wgetenv(L"HOMEPATH");
    if (drive != nullptr && path != nullptr) {
        // "c:", "\users\{user}"
        return path_t(reinterpret_cast<char16_t*>(drive)) + path_t(reinterpret_cast<char16_t*>(path));
    }

    // try SystemDrive
    home = _wgetenv(L"SystemDrive");
    if (home != nullptr) {
        return path_t(reinterpret_cast<char16_t*>(home));
    } else {
        return path_t(reinterpret_cast<const char16_t*>(L"c:"));
    }
}

#else                           // POSIX

static path_t gethomedir_impl()
{
    char *dir = std::getenv("HOME");
    if (dir != nullptr) {
        return path_t(dir);
    } else {
        // home not defined, root user
        return path_t("/");
    }
}

#endif

// VARIABLES
// ---------

path_t homedir = gethomedir_impl();

// FUNCTIONS
// ---------

path_t gethomedir()
{
    return homedir;
}

#if defined(OS_WINDOWS)          // BACKUP PATH

path_t gethomedirw()
{
    return gethomedir();
}


backup_path_t gethomedira()
{
    return path_to_backup_path(gethomedirw());
}

#endif

PYCPP_END_NAMESPACE
