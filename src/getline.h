//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Cross-platform getline implementation.
 */

// FUNCTIONS
// ---------

/** \brief Getline implementation for files from all platforms.
 *
 *  Getline implementation which stops at "\r", "\n", or both.
 */
std::istream& getline(std::istream& stream, std::string& line);
