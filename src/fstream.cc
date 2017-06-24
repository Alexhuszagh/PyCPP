//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "codec.h"
#include "fstream.h"


// FUNCTIONS
// ---------


/** \brief Convert C++ mode to C mode.
 */
std::string c_ios_mode(std::ios_base::openmode mode)
{
    using sios = std::ios_base;

    // static cast for clang warning, -WSwitch
    switch (static_cast<int>(mode & ~sios::ate)) {
        case sios::out:
        case sios::out | sios::trunc:
            return "w";
        case sios::out | sios::app:
        case sios::app:
            return "a";
        case sios::in:
            return "r";
        case sios::in | sios::out:
            return "r+";
        case sios::in | sios::out | sios::trunc:
            return "w+";
        case sios::in | sios::out | sios::app:
        case sios::in | sios::app:
            return "a+";
        case sios::out | sios::binary:
        case sios::out | sios::trunc | sios::binary:
            return "wb";
        case sios::out | sios::app | sios::binary:
        case sios::app | sios::binary:
            return "ab";
        case sios::in | sios::binary:
            return "rb";
        case sios::in | sios::out | sios::binary:
            return "r+b";
        case sios::in | sios::out | sios::trunc | sios::binary:
            return "w+b";
        case sios::in | sios::out | sios::app | sios::binary:
        case sios::in | sios::app | sios::binary:
            return "a+b";
        default:
            return "";
    }
}


/** \brief Get C FILE pointer from narrow filename.
 */
FILE * get_c_file(const std::string &narrow, std::ios_base::openmode mode)
{
    auto str = c_ios_mode(mode);
    if (str.size()) {
        return fopen(narrow.data(), str.data());
    }
    return nullptr;
}

#ifdef OS_WINDOWS


/** \brief Get C FILE pointer from wide filename.
 */
FILE * get_c_file(const std::wstring &wide,
    const std::ios_base::openmode mode)
{
    auto str = c_ios_mode(mode);
    if (str.size()) {
        auto data = reinterpet_cast<const wchar_t*>(codec_utf8_utf16(str).data());
        return _wfopen(wide.data(), data);
    }
    return nullptr;
}

#endif

// OBJECTS
// -------

#if HAVE_GCC                    // GCC

#endif
