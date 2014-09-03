/// @file utils.h
/// @brief support for tests
/// @author Jeff Perry <jeffsp@gmail.com>
/// @version 1.0
/// @date 2014-08-29

#pragma once

#include <iostream>
#include <stdexcept>
#include <sstream>

/// @brief Support for verify macro
inline void Verify (const char *e, const char *file, const unsigned line)
{
    std::stringstream s;
    s << "verification failed in " << file << ", line " << line << ": " << e;
    throw std::runtime_error (s.str ());
}

/// @brief Same as assert(), but independent of NDEBUG
#define verify(e) (void)((e) || (Verify (#e, __FILE__, __LINE__), 0))
