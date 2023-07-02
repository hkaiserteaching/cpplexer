//  Copyright (c) 2001 Daniel C. Nuffer
//  Copyright (c) 2001-2011 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../multi_pass_fwd.hpp"

#include "multi_pass.hpp"

#include <cstdlib>

namespace spirit::iterator_policies {

    ///////////////////////////////////////////////////////////////////////////
    //  class ref_counted
    //  Implementation of an OwnershipPolicy used by multi_pass.
    //
    //  Implementation modified from RefCounted class from the Loki library by
    //  Andrei Alexandrescu.
    ///////////////////////////////////////////////////////////////////////////
    struct ref_counted
    {
        ///////////////////////////////////////////////////////////////////////
        struct unique    // : detail::default_ownership_policy
        {
            void swap(unique&) noexcept {}

            // clone is called when a copy of the iterator is made, so
            // increment the ref-count.
            template <typename MultiPass>
            static void clone(MultiPass& mp)
            {
                if (nullptr != mp.shared())
                    ++mp.shared()->count;
            }

            // called when a copy is deleted. Decrement the ref-count. Return
            // value of true indicates that the last copy has been released.
            template <typename MultiPass>
            static bool release(MultiPass& mp)
            {
                return nullptr != mp.shared() && 0 == --mp.shared()->count;
            }

            // returns true if there is only one iterator in existence.
            // std_deque StoragePolicy will free it's buffered data if this
            // returns true.
            template <typename MultiPass>
            static bool is_unique(MultiPass const& mp)
            {
                return nullptr == mp.shared() || 1 == mp.shared()->count;
            }

            template <typename MultiPass>
            static void destroy(MultiPass&)
            {
            }
        };

        ////////////////////////////////////////////////////////////////////////
        struct shared
        {
            shared() = default;

            std::size_t count = 1;
        };
    };
}    // namespace spirit::iterator_policies
