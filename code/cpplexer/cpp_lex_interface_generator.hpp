/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Definition of the abstract lexer interface

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include "cpp_lex_interface.hpp"
#include "cpp_lex_token.hpp"
#include "file_position.hpp"
#include "language_support.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  new_lexer_gen: generates a new instance of the required C++ lexer
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename IteratorT, typename PositionT = util::file_position_type,
        typename TokenT = lex_token<PositionT>>
    struct new_lexer_gen
    {
        //  The NewLexer function allows the opaque generation of a new lexer object.
        //  It is coupled to the token type to allow to decouple the lexer/token
        //  configurations at compile time.
        static lex_input_interface<TokenT>* new_lexer(IteratorT const& first,
            IteratorT const& last, PositionT const& pos,
            language_support language);
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  The lex_input_interface_generator helps to instantiate a concrete lexer
    //  to be used by the Wave preprocessor module.
    //  This is done to allow compile time reduction.
    //
    ///////////////////////////////////////////////////////////////////////////////

    template <typename TokenT>
    struct lex_input_interface_generator : lex_input_interface<TokenT>
    {
        using position_type =
            typename lex_input_interface<TokenT>::position_type;

        lex_input_interface_generator() = default;
        ~lex_input_interface_generator() override = default;

        //  The new_lexer function allows the opaque generation of a new lexer object.
        //  It is coupled to the token type to allow to distinguish different
        //  lexer/token configurations at compile time.
        template <typename IteratorT>
        static lex_input_interface<TokenT>* new_lexer(IteratorT const& first,
            IteratorT const& last, position_type const& pos,
            language_support language)
        {
            return new_lexer_gen<IteratorT, position_type, TokenT>::new_lexer(
                first, last, pos, language);
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer
