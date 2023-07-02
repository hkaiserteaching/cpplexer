/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Re2C based C++ lexer

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

#include <cassert>

#include "../config.hpp"
#include "../convert_trigraphs.hpp"
#include "../cpp_lex_interface.hpp"
#include "../cpp_lex_interface_generator.hpp"
#include "../cpplexer_exceptions.hpp"
#include "../file_position.hpp"
#include "../language_support.hpp"
#include "../re2clex/cpp_re.hpp"
#include "../re2clex/scanner.hpp"
#include "../token_cache.hpp"
#include "../token_ids.hpp"
#include "../validate_universal_char.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer {

    namespace re2clex {

        ///////////////////////////////////////////////////////////////////////////////
        //
        //  encapsulation of the re2c based cpp lexer
        //
        ///////////////////////////////////////////////////////////////////////////////

        template <typename IteratorT,
            typename PositionT = util::file_position_type,
            typename TokenT = lex_token<PositionT>>
        class lexer
        {
        public:
            using token_type = TokenT;
            using string_type = typename token_type::string_type;

            lexer(IteratorT const& first, IteratorT const& last,
                PositionT const& pos, language_support language_);
            ~lexer();

            token_type& get(token_type&);
            void set_position(PositionT const& pos)
            {
                // set position has to change the file name and line number only
                filename = pos.get_file();
                scanner.line = pos.get_line();
                //        scanner.column = scanner.curr_column = pos.get_column();
                scanner.file_name = filename.c_str();
            }

            // error reporting from the re2c generated lexer
            static int report_error(
                Scanner<IteratorT> const* s, int errcode, char const*, ...);

        private:
            static char const* tok_names[];

            Scanner<IteratorT> scanner;
            string_type filename;
            string_type value;
            bool at_eof;
            language_support language;
            static token_cache<string_type> const cache;
        };

        ///////////////////////////////////////////////////////////////////////////////
        // initialize cpp lexer
        template <typename IteratorT, typename PositionT, typename TokenT>
        lexer<IteratorT, PositionT, TokenT>::lexer(IteratorT const& first,
            IteratorT const& last, PositionT const& pos,
            language_support language_)
          : scanner(first, last)
          , filename(pos.get_file())
          , at_eof(false)
          , language(language_)
        {
            using namespace std;    // some systems have memset in std

            scanner.line = pos.get_line();
            scanner.column = scanner.curr_column = pos.get_column();
            scanner.error_proc = report_error;
            scanner.file_name = filename.c_str();
            scanner.enable_ms_extensions = false;

#if WAVE_SUPPORT_VARIADICS_PLACEMARKERS != 0
            scanner.act_in_c99_mode = need_c99(language_);
#endif

            scanner.enable_import_keyword = false;

            scanner.detect_pp_numbers = need_prefer_pp_numbers(language_);
            scanner.single_line_only = need_single_line(language_);

#if WAVE_SUPPORT_CPP0X != 0
            scanner.act_in_cpp0x_mode = need_cpp0x(language_);
#else
            scanner.act_in_cpp0x_mode = false;
#endif

#if WAVE_SUPPORT_CPP2A != 0
            scanner.act_in_cpp2a_mode = need_cpp2a(language_);
            scanner.act_in_cpp0x_mode =
                need_cpp2a(language_) || need_cpp0x(language_);
#else
            scanner.act_in_cpp2a_mode = false;
#endif
        }

        template <typename IteratorT, typename PositionT, typename TokenT>
        lexer<IteratorT, PositionT, TokenT>::~lexer()
        {
            using namespace std;    // some systems have free in std
            free(scanner.bot);
        }

        ///////////////////////////////////////////////////////////////////////////////
        //  get the next token from the input stream
        template <typename IteratorT, typename PositionT, typename TokenT>
        TokenT& lexer<IteratorT, PositionT, TokenT>::get(TokenT& result)
        {
            if (at_eof)
                return result = token_type();    // return T_EOI

            std::size_t actline = scanner.line;
            auto id = static_cast<token_id>(scan(&scanner));

            switch (id)
            {
            case T_IDENTIFIER:
                // test identifier characters for validity (throws if invalid chars found)
                value = string_type(
                    (char const*) scanner.tok, scanner.cur - scanner.tok);
                if (!need_no_character_validation(language))
                    impl::validate_identifier_name(
                        value, actline, scanner.column, filename);
                break;

            case T_STRINGLIT:
            case T_CHARLIT:
            case T_RAWSTRINGLIT:
                // test literal characters for validity (throws if invalid chars found)
                value = string_type(
                    (char const*) scanner.tok, scanner.cur - scanner.tok);
                if (need_convert_trigraphs(language))
                    value = impl::convert_trigraphs(value);
                if (!need_no_character_validation(language))
                    impl::validate_literal(
                        value, actline, scanner.column, filename);
                break;

            case T_PP_HHEADER:
            case T_PP_QHEADER:
            case T_PP_INCLUDE:
                // convert to the corresponding ..._next token, if appropriate
                {
                    value = string_type(
                        (char const*) scanner.tok, scanner.cur - scanner.tok);

#if WAVE_SUPPORT_INCLUDE_NEXT != 0
                    // Skip '#' and whitespace and see whether we find an 'include_next' here.
                    typename string_type::size_type start =
                        value.find("include");
                    if (value.compare(start, 12, "include_next", 12) == 0)
                        id = static_cast<token_id>(id | AltTokenType);
#endif
                    break;
                }

            case T_LONGINTLIT:    // supported in C++11, C99 and long_long mode
                value = string_type(
                    (char const*) scanner.tok, scanner.cur - scanner.tok);
                if (!need_long_long(language))
                {
                    // syntax error: not allowed in C++ mode
                    WAVE_LEXER_THROW(lexing_exception,
                        invalid_long_long_literal, value.c_str(), actline,
                        scanner.column, filename.c_str());
                }
                break;

            case T_OCTALINT:
            case T_DECIMALINT:
            case T_HEXAINT:
            case T_INTLIT:
            case T_FLOATLIT:
            case T_FIXEDPOINTLIT:
            case T_CCOMMENT:
            case T_CPPCOMMENT:
            case T_SPACE:
            case T_SPACE2:
            case T_ANY:
            case T_PP_NUMBER:
                value = string_type(
                    (char const*) scanner.tok, scanner.cur - scanner.tok);
                break;

            case T_EOF:
                // T_EOF is returned as a valid token, the next call will return T_EOI,
                // i.e. the actual end of input
                at_eof = true;
                value.clear();
                break;

            case T_OR_TRIGRAPH:
            case T_XOR_TRIGRAPH:
            case T_LEFTBRACE_TRIGRAPH:
            case T_RIGHTBRACE_TRIGRAPH:
            case T_LEFTBRACKET_TRIGRAPH:
            case T_RIGHTBRACKET_TRIGRAPH:
            case T_COMPL_TRIGRAPH:
            case T_POUND_TRIGRAPH:
                if (need_convert_trigraphs(language))
                {
                    value = cache.get_token_value(BASEID_FROM_TOKEN(id));
                }
                else
                {
                    value = string_type(
                        (char const*) scanner.tok, scanner.cur - scanner.tok);
                }
                break;

            case T_ANY_TRIGRAPH:
                if (need_convert_trigraphs(language))
                {
                    value = impl::convert_trigraph(
                        string_type((char const*) scanner.tok));
                }
                else
                {
                    value = string_type(
                        (char const*) scanner.tok, scanner.cur - scanner.tok);
                }
                break;

            default:
                if (CATEGORY_FROM_TOKEN(id) != EXTCATEGORY_FROM_TOKEN(id) ||
                    IS_CATEGORY(id, UnknownTokenType))
                {
                    value = string_type(
                        (char const*) scanner.tok, scanner.cur - scanner.tok);
                }
                else
                {
                    value = cache.get_token_value(id);
                }
                break;
            }

            // the re2c lexer reports the new line number for newline tokens
            result = token_type(
                id, value, PositionT(filename, actline, scanner.column));

            return result;
        }

        template <typename IteratorT, typename PositionT, typename TokenT>
        int lexer<IteratorT, PositionT, TokenT>::report_error(
            Scanner<IteratorT> const* s, int errcode, char const* msg, ...)
        {
            assert(nullptr != s);
            assert(nullptr != msg);

            using namespace std;    // some system have vsprintf in namespace std

            char buffer[200];       // should be large enough
            va_list params;
            va_start(params, msg);
            vsprintf(buffer, msg, params);
            va_end(params);

            WAVE_LEXER_THROW_VAR(lexing_exception, errcode, buffer, s->line,
                s->column, s->file_name);
            //    UNREACHABLE_RETURN(0);
            return 0;
        }

        ///////////////////////////////////////////////////////////////////////////////
        //
        //  lex_functor
        //
        ///////////////////////////////////////////////////////////////////////////////

        template <typename IteratorT,
            typename PositionT = util::file_position_type,
            typename TokenT = typename lexer<IteratorT, PositionT>::token_type>
        class lex_functor : public lex_input_interface_generator<TokenT>
        {
        public:
            using token_type = TokenT;

            lex_functor(IteratorT const& first, IteratorT const& last,
                PositionT const& pos, language_support language)
              : re2c_lexer(first, last, pos, language)
            {
            }

            ~lex_functor() override = default;

            // get the next token from the input stream
            token_type& get(token_type& result) override
            {
                return re2c_lexer.get(result);
            }
            void set_position(PositionT const& pos) override
            {
                re2c_lexer.set_position(pos);
            }

        private:
            lexer<IteratorT, PositionT, TokenT> re2c_lexer;
        };

        ///////////////////////////////////////////////////////////////////////////
        template <typename IteratorT, typename PositionT, typename TokenT>
        token_cache<
            typename lexer<IteratorT, PositionT, TokenT>::string_type> const
            lexer<IteratorT, PositionT, TokenT>::cache = token_cache<
                typename lexer<IteratorT, PositionT, TokenT>::string_type>();
    }    // namespace re2clex

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  The new_lexer_gen<>::new_lexer function (declared in cpp_lex_interface.hpp)
    //  should be defined inline, if the lex_functor shouldn't be instantiated
    //  separately from the lex_iterator.
    //
    //  Separate (explicit) instantiation helps to reduce compilation time.
    //
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  The 'new_lexer' function allows the opaque generation of a new lexer object.
    //  It is coupled to the iterator type to allow to decouple the lexer/iterator
    //  configurations at compile time.
    //
    //  This function is declared inside the cpp_lex_token.hpp file, which is
    //  referenced by the source file calling the lexer and the source file, which
    //  instantiates the lex_functor. But it is defined here, so it will be
    //  instantiated only while compiling the source file, which instantiates the
    //  lex_functor. While the cpp_re2c_token.hpp file may be included everywhere,
    //  this file (cpp_re2c_lexer.hpp) should be included only once. This allows
    //  to decouple the lexer interface from the lexer implementation and reduces
    //  compilation time.
    //
    ///////////////////////////////////////////////////////////////////////////////

    template <typename IteratorT, typename PositionT, typename TokenT>
    lex_input_interface<TokenT>*
    new_lexer_gen<IteratorT, PositionT, TokenT>::new_lexer(
        IteratorT const& first, IteratorT const& last, PositionT const& pos,
        language_support language)
    {
        using re2clex::lex_functor;
        return new lex_functor<IteratorT, PositionT, TokenT>(
            first, last, pos, language);
    }

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer
