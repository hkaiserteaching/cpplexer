/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001 Daniel C. Nuffer.
    Copyright (c) 2001-2012 Hartmut Kaiser.
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include "../config.hpp"
#include "../re2clex/aq.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer::re2clex {

    template <typename Iterator>
    struct Scanner;
    using uchar = unsigned char;

    template <typename Iterator>
    struct Scanner
    {
        using ReportErrorProc = int (*)(
            Scanner const*, int errorcode, char const*, ...);

        Scanner(Iterator const& f, Iterator const& l)
          : first(f)
          , act(f)
          , last(l)
          , eol_offsets(aq_create())
        {
        }

        ~Scanner()
        {
            aq_terminate(eol_offsets);
        }

        Iterator first;       /* start of input buffer */
        Iterator act;         /* act position of input buffer */
        Iterator last;        /* end (one past last char) of input buffer */
        uchar* bot = nullptr; /* beginning of the current buffer */
        uchar* top = nullptr; /* top of the current buffer */
        /* when we read in the last buffer, will point 1 past the
                    end of the file, otherwise 0 */
        uchar* eof = nullptr;
        uchar* tok = nullptr; /* points to the beginning of the current token */
        uchar* ptr = nullptr; /* used for YYMARKER - saves backtracking info */
        /* saves the cursor (maybe is redundant with tok?) */
        uchar* cur = nullptr;
        /* used for YYLIMIT - points to the end of the buffer */
        /* (lim == top) except for the last buffer, it points to
                       the end of the input (lim == eof - 1) */
        uchar* lim = nullptr;
        std::size_t line = 0;        /* current line being lex'ed */
        std::size_t column = 0;      /* current token start column position */
        std::size_t curr_column = 0; /* current column position */
        /* must be != 0, this function is called to report an error */
        ReportErrorProc error_proc = nullptr;
        char const* file_name = nullptr;   /* name of the lex'ed file */
        aq_queue eol_offsets;
        bool enable_ms_extensions = false; /* enable MS extensions */
        bool act_in_c99_mode = false;      /* lexer works in C99 mode */
        /* lexer should prefer to detect pp-numbers */
        bool detect_pp_numbers = false;
        bool enable_import_keyword = false; /* recognize import as a keyword */
        /* don't report missing eol's in C++ comments */
        bool single_line_only = false;
        bool act_in_cpp0x_mode = false; /* lexer works in C++11 mode */
        bool act_in_cpp2a_mode = false; /* lexer works in C++20 mode */
    };

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer::re2clex
