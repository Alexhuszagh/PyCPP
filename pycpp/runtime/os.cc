//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/filesystem.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/runtime/os.h>
#include <pycpp/stream/fstream.h>
#include <pycpp/view/string.h>
#include <sstream>
#if defined(OS_WINDOWS)
#   include <windows.h>
#endif

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


#if defined(OS_LINUX)                   // LINUX

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
    std::stringstream sstream;
    sstream << input.rdbuf();
    auto str = sstream.str();

    return str.find("docker") != str.npos;
}


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
    } else if (view.substr(start, end) == "1") {
        return true;
    }

    return false;
}

#endif                                  // LINUX

// FUNCTIONS
// ---------


bool is_wine()
{
#if defined(OS_WINDOWS)                 // WINDOWS

    HMODULE hntdll = GetModuleHandle("ntdll.dll");
    if (!hntdll) {
        return false;
    }
    return bool(GetProcAddress(hntdll, "wine_get_version"));

#else                                   // POSIX
    return false;
#endif
}


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



bool is_container()
{
#if defined(OS_LINUX)                   // LINUX
    return !read_pid1();
#else                                   // NOT LINUX
    return false;
#endif
}


PYCPP_END_NAMESPACE
