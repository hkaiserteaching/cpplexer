//  Copyright (c) 2023 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include "cpplexer/cpp_lex_iterator.hpp"
#include "cpplexer/cpp_lex_token.hpp"
#include "cpplexer/cpplexer_exceptions.hpp"
#include "cpplexer/cpplexer_gen.hpp"

#include <string>

namespace cpplexer {

    using token_type = wave::cpplexer::lex_token<>;
    using lexer_type = wave::cpplexer::lex_iterator<token_type>;
    using position_type = token_type::position_type;

    using wave::token_id;

    using wave::BoolLiteralTokenType;
    using wave::CharacterLiteralTokenType;
    using wave::ExtParameterTokenType;
    using wave::FloatingLiteralTokenType;
    using wave::IdentifierTokenType;
    using wave::IntegerLiteralTokenType;
    using wave::KeywordTokenType;
    using wave::LiteralTokenType;
    using wave::OperatorTokenType;
    using wave::OptParameterTokenType;
    using wave::ParameterTokenType;
    using wave::PPConditionalTokenType;
    using wave::PPTokenType;
    using wave::StringLiteralTokenType;

    using wave::UnknownTokenType;
    using wave::EOFTokenType;
    using wave::EOLTokenType;
    using wave::WhiteSpaceTokenType;
    using wave::InternalTokenType;

    using wave::get_token_name;
    using wave::get_token_value;

    namespace detail {

        struct token_range
        {
            token_range(std::string const& infile);

            lexer_type begin();
            lexer_type end();

        private:
            std::string read_file();

            std::string input;
            std::string instr;
        };
    }    // namespace detail

    detail::token_range tokenize(std::string const& input);
}    // namespace cpplexer
