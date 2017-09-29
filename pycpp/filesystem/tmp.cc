//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/random.h>

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

#else                           // POSIX

static path_t gettempdir_impl()
{
    char *dir = std::getenv("TMPDIR");
    if (dir != nullptr) {
        return path_t(dir);
    }

    dir = std::getenv("TEMP");
    if (dir != nullptr) {
        return path_t(dir);
    }

    dir = std::getenv("TMP");
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

path_t gettempdir()
{
    return tempdir;
}


path_t gettempprefix()
{
    return tempprefix;
}


path_t gettempnam()
{
    path_list_t paths = {gettempdir(), gettempprefix() + gettempsuffix()};
    return join(paths);
}

#if defined(OS_WINDOWS)          // WINDOWS NT && BACKUP PATH

path_t gettempdirw()
{
    return gettempdir();
}


path_t gettempprefixw()
{
    return gettempprefix();
}


path_t gettempnamw()
{
    return gettempnam();
}


backup_path_t gettempdira()
{
    return path_to_backup_path(gettempdirw());
}


backup_path_t gettempprefixa()
{
    return path_to_backup_path(gettempprefixw());
}


backup_path_t gettempnama()
{
    return path_to_backup_path(gettempnam());
}

#else                           // POSIX

#endif

PYCPP_END_NAMESPACE
