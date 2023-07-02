/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Definition of the position_iterator and file_position templates

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include <ostream>
#include <string>
#include <utility>

#include "config.hpp"
#include "position_iterator.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::util {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  file_position
    //
    //  A structure to hold positional information. This includes the filename,
    //  line number and column number of a current token position.
    //
    ///////////////////////////////////////////////////////////////////////////////

    template <typename StringT>
    struct file_position
    {
        using string_type = StringT;

        file_position()
          : file()
          , line(1)
          , column(1)
        {
        }
        explicit file_position(
            string_type file_, std::size_t line_ = 1, std::size_t column_ = 1)
          : file(std::move(file_))
          , line(line_)
          , column(column_)
        {
        }

        // accessors
        [[nodiscard]] string_type const& get_file() const
        {
            return file;
        }

        [[nodiscard]] std::size_t get_line() const
        {
            return line;
        }

        [[nodiscard]] std::size_t get_column() const
        {
            return column;
        }

        void set_file(string_type const& file_)
        {
            file = file_;
        }
        void set_line(std::size_t line_)
        {
            line = line_;
        }
        void set_column(std::size_t column_)
        {
            column = column_;
        }

    private:
        string_type file;
        std::size_t line;
        std::size_t column;
    };

    template <typename StringT>
    bool operator==(
        file_position<StringT> const& lhs, file_position<StringT> const& rhs)
    {
        return lhs.get_column() == rhs.get_column() &&
            lhs.get_line() == rhs.get_line() &&
            lhs.get_file() == rhs.get_file();
    }

    template <typename StringT>
    std::ostream& operator<<(std::ostream& o, file_position<StringT> const& pos)
    {
        o << pos.get_file() << ":" << pos.get_line() << ":" << pos.get_column();
        return o;
    }

    using file_position_type = file_position<WAVE_STRINGTYPE>;

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  position_iterator
    //
    //  The position_iterator used by Wave is now based on the corresponding Spirit
    //  type. This type is used with our own file_position though. The needed
    //  specialization of the boost::spirit::classic::position_policy class is
    //  provided below.
    //
    ///////////////////////////////////////////////////////////////////////////////

    template <typename IteratorT, typename PositionT>
    struct position_iterator
      : spirit::classic::position_iterator<IteratorT, PositionT>
    {
        using base_type =
            spirit::classic::position_iterator<IteratorT, PositionT>;

        position_iterator() = default;

        position_iterator(
            IteratorT const& begin, IteratorT const& end, PositionT const& pos)
          : base_type(begin, end, pos)
        {
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::util

///////////////////////////////////////////////////////////////////////////////
template <>
class spirit::classic::position_policy<wave::util::file_position_type>
{
public:
    position_policy()
      : m_CharsPerTab(4)
    {
    }

    static void next_line(wave::util::file_position_type& pos)
    {
        pos.set_line(pos.get_line() + 1);
        pos.set_column(1);
    }

    void set_tab_chars(unsigned int chars)
    {
        m_CharsPerTab = chars;
    }

    static void next_char(wave::util::file_position_type& pos)
    {
        pos.set_column(pos.get_column() + 1);
    }

    void tabulation(wave::util::file_position_type& pos) const
    {
        pos.set_column(pos.get_column() + m_CharsPerTab -
            (pos.get_column() - 1) % m_CharsPerTab);
    }

private:
    unsigned int m_CharsPerTab;
};
