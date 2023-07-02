/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    Explicit instantiation of the lex_functor generation function

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include "config.hpp"
#include "cpp_lex_iterator.hpp"
#include "cpp_lex_token.hpp"
#include "token_ids.hpp"

#include <string>

///////////////////////////////////////////////////////////////////////////////
//  The following file needs to be included only once throughout the whole
//  program.
#include "re2clex/cpp_re2c_lexer.hpp"

template struct wave::cpplexer::new_lexer_gen<std::string::iterator>;
template struct wave::cpplexer::new_lexer_gen<std::string::const_iterator>;
