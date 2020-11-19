////////////////////////////////////////////////////////////////////////////////
// IME - Infinite Motion Engine
//
// Copyright (c) 2020 Kwena Mashamaite (kmash.ime@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#ifndef IME_CONFIG_HPP
#define IME_CONFIG_HPP

// This file is generated by CMake and must not be edited directly.

// Include the SFML and TGUI config header to check the library version
#include <SFML/Config.hpp>
#include <TGUI/Config.hpp>

#if SFML_VERSION_MAJOR < 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5) || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR == 5 && SFML_VERSION_PATCH < 1)
    #error IME requires SFML >= 2.5.1
#endif

#if TGUI_VERSION_MAJOR == 0 && TGUI_VERSION_MINOR < 9
    #error IME requires TGUI >= 0.9.0
#endif

// Detect the platform, to enable platform-specific code
#if defined(_WIN32)
    #define IME_SYSTEM_WINDOWS // Windows
#endif

// IME will link in the same way as SFML and TGUI, unless IME_DYNAMIC or IME_STATIC is defined
#if !defined(IME_DYNAMIC) && !defined(IME_STATIC)
    #ifdef SFML_STATIC
        #define IME_STATIC
    #endif
#endif

#ifndef IME_STATIC
    #ifdef IME_SYSTEM_WINDOWS
        // Windows compilers need specific (and different) keywords for export and import
        #ifdef IME_EXPORTS
            #define IME_API __declspec(dllexport)
        #else
            #define IME_API __declspec(dllimport)
        #endif
    #endif
#else
    // Static build doesn't need import/export macros
    #define IME_API
    #define IME_API
#endif

// Version of the library
#define IME_VERSION_MAJOR 1
#define IME_VERSION_MINOR 0
#define IME_VERSION_PATCH 0

#endif // IME_CONFIG_H