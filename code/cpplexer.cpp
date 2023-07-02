//  Copyright (c) 2023 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "cpplexer.hpp"

#include <fstream>
#include <string>

namespace cpplexer {

    namespace detail {

        std::string token_range::read_file()
        {
            //  Open and read in the specified input file.
            std::ifstream instream(input);

            if (!instream.is_open())
            {
                throw std::runtime_error("Could not open input file: " + input);
            }

            instream.unsetf(std::ios::skipws);

            return {std::istreambuf_iterator<char>(instream.rdbuf()),
                std::istreambuf_iterator<char>()};
        }

        token_range::token_range(std::string const& input)
          : input(input)
          , instr(read_file())
        {
        }

        lexer_type token_range::begin()
        {
            position_type const pos(input);

            return lexer_type(instr.begin(), instr.end(), pos,
                wave::language_support(wave::support_cpp | wave::support_cpp20 |
                    wave::support_option_long_long));
        }

        lexer_type token_range::end()
        {
            return lexer_type();
        }

    }    // namespace detail

    detail::token_range tokenize(std::string const& input)
    {
        return detail::token_range(input);
    }
}    // namespace cpplexer
