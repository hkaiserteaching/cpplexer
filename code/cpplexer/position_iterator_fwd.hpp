/*=============================================================================
    Copyright (c) 2006 Tobias Schwinger
    Copyright (c) 2002-2006 Hartmut Kaiser
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <iterator>    // for std::iterator_traits
#include <string>

namespace spirit::classic {

    template <typename String = std::string>
    struct file_position_base;

    using file_position = file_position_base<std::string>;

    template <typename String = std::string>
    struct file_position_without_column_base;

    using file_position_without_column = file_position_without_column_base<std::string>;

    template <typename ForwardIteratorT,
        typename PositionT = file_position_base<std::basic_string<
            typename std::iterator_traits<ForwardIteratorT>::value_type>>,
        typename SelfT = void>
    class position_iterator;

    template <typename PositionT>
    class position_policy;
}    // namespace spirit::classic
