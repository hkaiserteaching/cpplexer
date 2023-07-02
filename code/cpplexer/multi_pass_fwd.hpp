/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright (c) 2001-2011 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <type_traits>

#include "multi_pass_wrapper.hpp"

namespace spirit {

    namespace iterator_policies {
        // input policies
        struct buffering_input_iterator;
        struct split_functor_input;

        // ownership policies
        struct ref_counted;

        // checking policies
        struct no_check;

        // storage policies
        struct split_std_deque;

        // policy combiner
        template <typename Ownership = ref_counted,
            typename Checking = no_check,
            typename Input = buffering_input_iterator,
            typename Storage = split_std_deque>
        struct default_policy;
    }    // namespace iterator_policies

    template <typename T,
        typename Policies = iterator_policies::default_policy<>>
    class multi_pass;

    template <typename T, typename Policies>
    void swap(multi_pass<T, Policies>& x, multi_pass<T, Policies>& y) noexcept;
}    // namespace spirit

namespace spirit::traits {

    // declare special functions allowing to integrate any multi_pass iterator
    // with expectation points

    // multi_pass iterators require special handling (for the non-specialized
    // versions of these functions see support/multi_pass_wrapper.hpp)
    template <typename T, typename Policies>
    void clear_queue(multi_pass<T, Policies>&,
        clear_mode mode = clear_mode::clear_if_enabled);

    template <typename T, typename Policies>
    void inhibit_clear_queue(multi_pass<T, Policies>&, bool);

    template <typename T, typename Policies>
    bool inhibit_clear_queue(multi_pass<T, Policies>&);

    // Helper template to recognize a multi_pass iterator. This specialization
    // will be instantiated for any multi_pass iterator.
    template <typename T, typename Policies>
    struct is_multi_pass<multi_pass<T, Policies>> : std::true_type
    {
    };
}    // namespace spirit::traits
