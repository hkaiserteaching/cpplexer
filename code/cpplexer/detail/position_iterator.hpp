/*=============================================================================
    Copyright (c) 2002 Juan Carlos Arevalo-Baeza
    Copyright (c) 2002-2006 Hartmut Kaiser
    Copyright (c) 2003 Giovanni Bajo
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <iterator>    // for std::iterator_traits
#include <type_traits>

#include "../iterator_adaptor.hpp"

namespace spirit::classic {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  position_policy<file_position_without_column>
    //
    //  Specialization to handle file_position_without_column. Only take care of
    //  newlines since no column tracking is needed.
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename String>
    class position_policy<file_position_without_column_base<String>>
    {
    public:
        static void next_line(file_position_without_column_base<String>& pos)
        {
            ++pos.line;
        }

        static constexpr void set_tab_chars(unsigned int /*chars*/) noexcept {}
        static constexpr void next_char(
            file_position_without_column_base<String>& /*pos*/) noexcept
        {
        }
        static constexpr void tabulation(
            file_position_without_column_base<String>& /*pos*/) noexcept
        {
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  position_policy<file_position>
    //
    //  Specialization to handle file_position. Track characters and tabulation
    //  to compute the current column correctly.
    //
    //  Default tab size is 4. You can change this with the set_tabchars member
    //  of position_iterator.
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename String>
    class position_policy<file_position_base<String>>
    {
    public:
        position_policy() = default;

        static void next_line(file_position_base<String>& pos) noexcept
        {
            ++pos.line;
            pos.column = 1;
        }

        void set_tab_chars(unsigned int chars) noexcept
        {
            m_CharsPerTab = chars;
        }

        static void next_char(file_position_base<String>& pos) noexcept
        {
            ++pos.column;
        }

        void tabulation(file_position_base<String>& pos) noexcept
        {
            pos.column += m_CharsPerTab - (pos.column - 1) % m_CharsPerTab;
        }

    private:
        unsigned int m_CharsPerTab = 4;
    };

    namespace iterator_::impl {

        template <typename T>
        struct make_const : std::add_const<T>
        {
        };

        template <typename T>
        struct make_const<T&>
        {
            using type = std::add_const_t<T>&;
        };

        ///////////////////////////////////////////////////////////////////////////////
        //
        //  position_iterator_base_generator
        //
        //  Meta-function to generate the iterator type using iterator_adaptors,
        //  hiding all the meta-programming thunking code in it. It is used
        //  mainly to keep the public interface (position_iterator) cleaner.
        //
        ///////////////////////////////////////////////////////////////////////////////
        template <typename MainIterT, typename ForwardIterT, typename PositionT>
        struct position_iterator_base_generator
        {
        private:
            using traits = std::iterator_traits<ForwardIterT>;
            using value_type = typename traits::value_type;
            using iter_category_t = typename traits::iterator_category;
            using reference = typename traits::reference;

            // Position iterator is always a non-mutable iterator
            using const_value_type = std::add_const_t<value_type>;

        public:
            // Check if the MainIterT is void. If it's void, it means that the actual
            //  self type is position_iterator. Otherwise, it's a real type we
            //  must use
            using main_iter_t =
                std::conditional_t<std::is_same_v<MainIterT, void>,
                    position_iterator<ForwardIterT, PositionT, void>,
                    MainIterT>;

            using type = hpx::iterator_adaptor<main_iter_t, ForwardIterT,
                const_value_type, hpx::forward_traversal_tag,
                typename make_const<reference>::type>;
        };
    }    // namespace iterator_::impl
}    // namespace spirit::classic
