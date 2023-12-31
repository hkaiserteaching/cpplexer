/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include <cassert>
#include <exception>
#include <sstream>
#include <string>

#include "config.hpp"

///////////////////////////////////////////////////////////////////////////////
// helper macro for throwing exceptions
#define WAVE_LEXER_THROW(cls, code, msg, line, column, name)                   \
    {                                                                          \
        using namespace wave;                                                  \
        std::stringstream stream;                                              \
        stream << cls::severity_text(cls::code) << ": "                        \
               << cls::error_text(cls::code);                                  \
        if ((msg)[0] != 0)                                                     \
            stream << ": " << (msg);                                           \
        stream << std::ends;                                                   \
        throw cls(stream.str().c_str(), cls::code, line, column, name);        \
    }                                                                          \
    /**/

#define WAVE_LEXER_THROW_VAR(cls, codearg, msg, line, column, name)            \
    {                                                                          \
        using namespace wave;                                                  \
        cls::error_code code = static_cast<cls::error_code>(codearg);          \
        std::stringstream stream;                                              \
        stream << cls::severity_text(code) << ": " << cls::error_text(code);   \
        if ((msg)[0] != 0)                                                     \
            stream << ": " << (msg);                                           \
        stream << std::ends;                                                   \
        throw cls(stream.str().c_str(), code, line, column, name);             \
    }                                                                          \
    /**/

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer {

    ///////////////////////////////////////////////////////////////////////////////
    // exception severity
    namespace util {

        enum severity
        {
            severity_remark = 0,
            severity_warning,
            severity_error,
            severity_fatal
        };

        inline char const* get_severity(severity level)
        {
            static char const* severity_text[] = {
                "remark",        // severity_remark
                "warning",       // severity_warning
                "error",         // severity_error
                "fatal error"    // severity_fatal
            };
            assert(severity_remark <= level && level <= severity_fatal);
            return severity_text[level];
        }
    }    // namespace util

    ///////////////////////////////////////////////////////////////////////////////
    //  cpplexer_exception, the base class for all specific C++ lexer exceptions
    class cpplexer_exception : public std::exception
    {
    public:
        cpplexer_exception(std::size_t line_, std::size_t column_,
            char const* filename_) noexcept
          : line(line_)
          , column(column_)
        {
            unsigned int off = 0;
            while (off < sizeof(filename) - 1 && *filename_)
                filename[off++] = *filename_++;
            filename[off] = 0;
        }
        ~cpplexer_exception() override = default;

        char const* what() const noexcept override = 0;    // to be overloaded
        virtual char const* description() const noexcept = 0;
        virtual int get_errorcode() const noexcept = 0;
        virtual int get_severity() const noexcept = 0;
        virtual bool is_recoverable() const noexcept = 0;

        [[nodiscard]] std::size_t line_no() const noexcept
        {
            return line;
        }

        [[nodiscard]] std::size_t column_no() const noexcept
        {
            return column;
        }

        [[nodiscard]] char const* file_name() const noexcept
        {
            return filename;
        }

    protected:
        char filename[512]{};
        std::size_t line;
        std::size_t column;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // lexing_exception error
    class lexing_exception : public cpplexer_exception
    {
    public:
        enum error_code
        {
            unexpected_error = 0,
            universal_char_invalid = 1,
            universal_char_base_charset = 2,
            universal_char_not_allowed = 3,
            invalid_long_long_literal = 4,
            generic_lexing_error = 5,
            generic_lexing_warning = 6
        };

        lexing_exception(char const* what_, error_code code, std::size_t line_,
            std::size_t column_, char const* filename_) noexcept
          : cpplexer_exception(line_, column_, filename_)
          , level(severity_level(code))
          , code(code)
        {
            unsigned int off = 0;
            while (off < sizeof(buffer) - 1 && *what_)
                buffer[off++] = *what_++;
            buffer[off] = 0;
        }
        ~lexing_exception() override = default;

        [[nodiscard]] char const* what() const noexcept override
        {
            return "lexing_exception";
        }

        [[nodiscard]] char const* description() const noexcept override
        {
            return buffer;
        }

        [[nodiscard]] int get_severity() const noexcept override
        {
            return level;
        }

        [[nodiscard]] int get_errorcode() const noexcept override
        {
            return code;
        }

        [[nodiscard]] bool is_recoverable() const noexcept override
        {
            switch (get_errorcode())
            {
            case lexing_exception::universal_char_invalid:
            case lexing_exception::universal_char_base_charset:
            case lexing_exception::universal_char_not_allowed:
            case lexing_exception::invalid_long_long_literal:
            case lexing_exception::generic_lexing_warning:
            case lexing_exception::generic_lexing_error:
                return true;    // for now allow all exceptions to be recoverable

            case lexing_exception::unexpected_error:
            default:
                break;
            }
            return false;
        }

        static char const* error_text(int code)
        {
            // error texts in this array must appear in the same order as the items in
            // the error enum above
            static char const* preprocess_exception_errors[] = {
                "unexpected error (should not happen)",    // unexpected_error
                "universal character name specifies an invalid "
                "character",    // universal_char_invalid
                "a universal character name cannot designate a "
                "character in the basic character set",    // universal_char_base_charset
                "this universal character is not allowed in an "
                "identifier",    // universal_char_not_allowed
                "long long suffixes are not allowed in pure C++ mode, "
                "enable long_long mode to allow these",    // invalid_long_long_literal
                "generic lexer error",     // generic_lexing_error
                "generic lexer warning"    // generic_lexing_warning
            };
            return preprocess_exception_errors[code];
        }

        static util::severity severity_level(int code)
        {
            static util::severity preprocess_exception_severity[] = {
                util::severity_fatal,      // unexpected_error
                util::severity_error,      // universal_char_invalid
                util::severity_error,      // universal_char_base_charset
                util::severity_error,      // universal_char_not_allowed
                util::severity_warning,    // invalid_long_long_literal
                util::severity_error,      // generic_lexing_error
                util::severity_warning     // invalid_long_long_literal
            };
            return preprocess_exception_severity[code];
        }
        static char const* severity_text(int code)
        {
            return util::get_severity(severity_level(code));
        }

    private:
        char buffer[512];
        util::severity level;
        error_code code;
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  The is_recoverable() function allows to decide, whether it is possible
    //  simply to continue after a given exception was thrown by Wave.
    //
    //  This is kind of a hack to allow to recover from certain errors as long as
    //  Wave doesn't provide better means of error recovery.
    //
    ///////////////////////////////////////////////////////////////////////////////
    inline bool is_recoverable(lexing_exception const& e)
    {
        return e.is_recoverable();
    }

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer
