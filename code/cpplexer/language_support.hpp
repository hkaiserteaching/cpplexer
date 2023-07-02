/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    Definition of the various language support constants

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include "config.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave {

    enum language_support
    {
        //  support flags for C++98
        support_normal = 0x01,
        support_cpp = support_normal,

        support_option_long_long = 0x02,

#if WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
        //  support flags for C99
        support_option_variadics = 0x04,
        support_c99 =
            support_option_variadics | support_option_long_long | 0x08,
#endif
#if WAVE_SUPPORT_CPP0X != 0
        //  support flags for C++11
        support_option_no_newline_at_end_of_file = 0x20,

        support_cpp0x = support_option_variadics | support_option_long_long |
            support_option_no_newline_at_end_of_file | 0x10,
        support_cpp11 = support_cpp0x,
#if WAVE_SUPPORT_CPP1Z != 0
        // support flags for C++17
        support_option_has_include = 0x10000,

        support_cpp1z = support_option_variadics | support_option_long_long |
            support_option_no_newline_at_end_of_file |
            support_option_has_include | 0x20000,
        support_cpp17 = support_cpp1z,
#if WAVE_SUPPORT_CPP2A != 0
        //  support flags for C++20
        support_option_va_opt = 0x40000,

        support_cpp2a = support_option_variadics | support_option_long_long |
            support_option_no_newline_at_end_of_file |
            support_option_has_include | support_option_va_opt | 0x80000,
        support_cpp20 = support_cpp2a,
#endif
#endif
#endif

        support_option_mask = 0xFFC0,
        support_option_emit_contnewlines = 0x0040,
        support_option_insert_whitespace = 0x0080,
        support_option_preserve_comments = 0x0100,
        support_option_no_character_validation = 0x0200,
        support_option_convert_trigraphs = 0x0400,
        support_option_single_line = 0x0800,
        support_option_prefer_pp_numbers = 0x1000,
        support_option_emit_line_directives = 0x2000,
        support_option_include_guard_detection = 0x4000,
        support_option_emit_pragma_directives = 0x8000
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  need_cpp
    //
    //      Extract, if the language to support is C++98
    //
    ///////////////////////////////////////////////////////////////////////////////
    constexpr bool need_cpp(language_support language) noexcept
    {
        return (language & ~support_option_mask) == support_cpp;
    }

#if WAVE_SUPPORT_CPP0X != 0
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  need_cpp0x
    //
    //      Extract, if the language to support is C++11
    //
    ///////////////////////////////////////////////////////////////////////////////

    constexpr bool need_cpp0x(language_support language) noexcept
    {
        return (language & ~support_option_mask) == support_cpp0x;
    }

#else

    constexpr bool need_cpp0x(language_support language) noexcept
    {
        return false;
    }

#endif

#if WAVE_SUPPORT_CPP2A != 0
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  need_cpp2a
    //
    //      Extract if the language to support is C++20
    //
    ///////////////////////////////////////////////////////////////////////////////

    constexpr bool need_cpp2a(language_support language) noexcept
    {
        return (language & ~support_option_mask) == support_cpp2a;
    }

#else

    constexpr bool need_cpp2a(language_support language) noexcept
    {
        return false;
    }

#endif

#if WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
    ///////////////////////////////////////////////////////////////////////////////
    //
    //  need_c99
    //
    //      Extract, if the language to support is C99
    //
    ///////////////////////////////////////////////////////////////////////////////
    constexpr bool need_c99(language_support language) noexcept
    {
        return (language & ~support_option_mask) == support_c99;
    }

#else     // WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0

    ///////////////////////////////////////////////////////////////////////////////
    constexpr bool need_variadics(language_support language) noexcept
    {
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    constexpr language_support enable_variadics(
        language_support language, bool enable = true) noexcept
    {
        return language;
    }

    //////////////////////////////////////////////////////////////////////////////
    constexpr bool need_c99(language_support language) noexcept
    {
        return false;
    }

#endif    // WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  get_support_options
    //
    //      Set preserve comments support in the language to support
    //
    ///////////////////////////////////////////////////////////////////////////////
    constexpr language_support get_support_options(
        language_support language) noexcept
    {
        return static_cast<language_support>(language & support_option_mask);
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  set_support_options
    //
    //      Set language option (for fine tuning of lexer behavior)
    //
    ///////////////////////////////////////////////////////////////////////////////
    constexpr language_support set_support_options(
        language_support language, language_support option) noexcept
    {
        return static_cast<language_support>(
            (language & ~support_option_mask) | (option & support_option_mask));
    }

///////////////////////////////////////////////////////////////////////////////
//  Get and set different language options
#define WAVE_NEED_OPTION(option)                                               \
    constexpr bool need_##option(language_support language) noexcept           \
    {                                                                          \
        return (language & support_option_##option) ? true : false;            \
    }                                                                          \
    /**/

#define WAVE_ENABLE_OPTION(option)                                             \
    constexpr language_support enable_##option(                                \
        language_support language, bool enable = true) noexcept                \
    {                                                                          \
        if (enable)                                                            \
            return static_cast<language_support>(                              \
                language | support_option_##option);                           \
        return static_cast<language_support>(                                  \
            language & ~support_option_##option);                              \
    }                                                                          \
    /**/

#define WAVE_OPTION(option)                                                    \
    WAVE_NEED_OPTION(option)                                                   \
    WAVE_ENABLE_OPTION(option)                                                 \
    /**/

    ///////////////////////////////////////////////////////////////////////////////
    WAVE_OPTION(long_long)            // support_option_long_long
    WAVE_OPTION(
        no_character_validation)      // support_option_no_character_validation
    WAVE_OPTION(preserve_comments)    // support_option_preserve_comments
    WAVE_OPTION(prefer_pp_numbers)    // support_option_prefer_pp_numbers
    WAVE_OPTION(emit_line_directives)    // support_option_emit_line_directives
    WAVE_OPTION(single_line)             // support_option_single_line
    WAVE_OPTION(convert_trigraphs)       // support_option_convert_trigraphs
#if WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
    WAVE_OPTION(variadics)    // support_option_variadics
#endif
#if WAVE_SUPPORT_VA_OPT != 0
    WAVE_OPTION(va_opt)    // support_option_va_opt
#endif
#if WAVE_EMIT_PRAGMA_DIRECTIVES != 0
    WAVE_OPTION(
        emit_pragma_directives)       // support_option_emit_pragma_directives
#endif
    WAVE_OPTION(insert_whitespace)    // support_option_insert_whitespace
    WAVE_OPTION(emit_contnewlines)    // support_option_emit_contnewlines
#if WAVE_SUPPORT_CPP0X != 0
    WAVE_OPTION(
        no_newline_at_end_of_file)    // support_no_newline_at_end_of_file
#endif
#if WAVE_SUPPORT_HAS_INCLUDE != 0
    WAVE_OPTION(has_include)    // support_option_has_include
#endif

#undef WAVE_NEED_OPTION
#undef WAVE_ENABLE_OPTION
#undef WAVE_OPTION

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave
