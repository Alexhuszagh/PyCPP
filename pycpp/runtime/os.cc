//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/runtime/os.h>
#include <pycpp/stl/fstream.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/stl/string_view.h>
#if defined(OS_WINDOWS)
#   include <pycpp/windows/winapi.h>
#endif

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


#if defined(OS_LINUX)                   // LINUX

#if BUILD_FILESYSTEM                    // BUILD_FILESYTEM

/**
 *  \brief Check if the `.dockerinit` file exists (deprecated).
 */
static bool read_dockerinit()
{
    return exists("/.dockerinit");
}


/**
 *  \brief Check if the `.dockerenv` file exists.
 */
static bool read_dockerenv()
{
    return exists("/.dockerenv");
}


/**
 *  \brief Check if "docker" is in `/proc/1/cgroup`.
 */
static bool read_proc_cgroup()
{
    if (!exists("/proc/1/cgroup")) {
        return false;
    }

    ifstream input("/proc1/cgroup");
    stringstream sstream;
    sstream << input.rdbuf();
    auto str = sstream.str();

    return str.find("docker") != str.npos;
}

#endif                                  // BUILD_FILESYSTEM


/**
 *  \brief Check if PID1 is the init system (false for containers).
 */
static bool read_pid1()
{
    // this **has** to exist.
    ifstream input("/proc/1/sched");
    std::string line;
    std::getline(input, line);

    // init system takes the following format for the native host
    // init (1, #threads: 1)
    // systemd (1, #threads: 1)
    string_view view(line);
    size_t start = view.find("(");
    size_t end = view.find(",", start);
    if (start == view.npos || end == view.npos) {
        return false;
    } else if (view.substr(start, end) == string_view("1")) {
        return true;
    }

    return false;
}

#endif                                  // LINUX

// FUNCTIONS
// ---------


bool is_wine() noexcept
{
#if defined(OS_WINDOWS)                 // WINDOWS

    HMODULE hntdll = GetModuleHandleA("ntdll.dll");
    if (!hntdll) {
        return false;
    }
    return bool(GetProcAddress(hntdll, "wine_get_version"));

#else                                   // POSIX
    return false;
#endif
}


bool is_container()
{
#if defined(OS_LINUX)                   // LINUX
    return !read_pid1();
#else                                   // NOT LINUX
    return false;
#endif
}


#if BUILD_FILESYSTEM                    // BUILD_FILESYSTEM

/**
 *  This algorithm is prone to break, and aims to be stable among
 *  nearly every docker version available.
 */
bool is_docker()
{
#if defined(OS_LINUX)                   // LINUX

    if (read_dockerinit()) {
        return true;
    } else if (read_dockerenv()) {
        return true;
    } else if (read_proc_cgroup()) {
        return true;
    }
    return false;

#else                                   // NOT LINUX
    return false;
#endif
}

#endif                                  // BUILD_FILESYSTEM


PYCPP_END_NAMESPACE
