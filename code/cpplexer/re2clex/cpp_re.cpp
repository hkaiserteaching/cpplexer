/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Copyright (c) 2001 Daniel C. Nuffer
    Copyright (c) 2001-2012 Hartmut Kaiser.
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <cstddef>

#include "cpp_re.hpp"
#include "scanner.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer::re2clex {

    bool is_backslash(const uchar* p, const uchar* end, int& len)
    {
        if (*p == '\\')
        {
            len = 1;
            return true;
        }
        if (*p == '?' && *(p + 1) == '?' &&
            (p + 2 < end && *(p + 2) == '/'))
        {
            len = 3;
            return true;
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //  Special wrapper class holding the current cursor position
    uchar_wrapper::uchar_wrapper(uchar* base_cursor, std::size_t column)
      : base_cursor(base_cursor)
      , column(column)
    {
    }

    uchar_wrapper& uchar_wrapper::operator++()
    {
        ++base_cursor;
        ++column;
        return *this;
    }

    uchar_wrapper& uchar_wrapper::operator--()
    {
        --base_cursor;
        --column;
        return *this;
    }

    uchar uchar_wrapper::operator*() const
    {
        return *base_cursor;
    }

    uchar_wrapper::operator uchar*() const
    {
        return base_cursor;
    }

    std::ptrdiff_t operator-(uchar_wrapper const& lhs, uchar_wrapper const& rhs)
    {
        return lhs.base_cursor - rhs.base_cursor;
    }
}    // namespace wave::cpplexer::re2clex
