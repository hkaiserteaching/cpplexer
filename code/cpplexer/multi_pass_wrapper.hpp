//  Copyright (c) 2001-2011 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <type_traits>

namespace spirit::traits {

    // declare special functions allowing to integrate any multi_pass iterator
    // with expectation points

    // normal iterators require no special handling
    enum class clear_mode
    {
        clear_if_enabled,
        clear_always
    };

    template <typename Iterator>
    void clear_queue(
        Iterator&, clear_mode /*mode*/ = clear_mode::clear_if_enabled)
    {
    }

    template <typename Iterator>
    void inhibit_clear_queue(Iterator&, bool)
    {
    }

    template <typename Iterator>
    bool inhibit_clear_queue(Iterator&)
    {
        return false;
    }

    // Helper template to recognize a multi_pass iterator. This specialization
    // will be instantiated for any non-multi_pass iterator.
    template <typename Iterator>
    struct is_multi_pass : std::false_type
    {
    };
}    // namespace spirit::traits
