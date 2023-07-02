/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Definition of the abstract lexer interface

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include "config.hpp"
#include "file_position.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  The lex_input_interface decouples the lex_iterator_shim from the actual
    //  lexer. This is done to allow compile time reduction.
    //  Thanks to JCAB for having this idea.
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename TokenT>
    struct lex_input_interface
    {
        using position_type = typename TokenT::position_type;

        lex_input_interface() = default;

        virtual ~lex_input_interface() = default;

        virtual TokenT& get(TokenT&) = 0;
        virtual void set_position(position_type const& pos) = 0;
    };

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer
