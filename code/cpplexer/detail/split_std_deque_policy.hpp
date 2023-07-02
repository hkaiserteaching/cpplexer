//  Copyright (c) 2001 Daniel C. Nuffer
//  Copyright (c) 2001-2011 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../multi_pass_fwd.hpp"

#include "multi_pass.hpp"

#include <cassert>
#include <utility>
#include <vector>

namespace spirit::iterator_policies {

    ///////////////////////////////////////////////////////////////////////////
    //  class split_std_deque
    //
    //  Implementation of the StoragePolicy used by multi_pass
    //  This stores all data in a std::vector (despite its name), and keeps an
    //  offset to the current position. It stores all the data unless there is
    //  only one iterator using the queue.
    //
    ///////////////////////////////////////////////////////////////////////////
    struct split_std_deque
    {
        enum
        {
            threshold = 16
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename Value>
        class unique    //: public detail::default_storage_policy
        {
            using queue_type = std::vector<Value>;

        protected:
            unique() = default;

            unique(unique const& x)
              : queued_position(x.queued_position)
            {
            }

            void swap(unique& x) noexcept
            {
                using std::swap;
                swap(queued_position, x.queued_position);
            }

            // This is called when the iterator is de-referenced.  It's a
            // template method so we can recover the type of the multi_pass
            // iterator and call advance_input and input_is_valid.
            template <typename MultiPass>
            static typename MultiPass::reference dereference(
                MultiPass const& mp)
            {
                queue_type& queue = mp.shared()->queued_elements;
                typename queue_type::size_type size = queue.size();

                assert(mp.queued_position <= size);

                if (mp.queued_position == size)
                {
                    // check if this is the only iterator
                    if (size >= threshold && MultiPass::is_unique(mp))
                    {
                        // free up the memory used by the queue.
                        queue.clear();
                        mp.queued_position = 0;
                    }
                    return MultiPass::get_input(mp);
                }

                return queue[mp.queued_position];
            }

            // This is called when the iterator is incremented. It's a template
            // method so we can recover the type of the multi_pass iterator
            // and call is_unique and advance_input.
            template <typename MultiPass>
            static void increment(MultiPass& mp)
            {
                queue_type& queue = mp.shared()->queued_elements;
                typename queue_type::size_type size = queue.size();

                assert(mp.queued_position <= size);

                if (mp.queued_position == size)
                {
                    // check if this is the only iterator
                    if (size >= threshold && MultiPass::is_unique(mp))
                    {
                        // free up the memory used by the queue. we avoid
                        // clearing the queue on every increment, though,
                        // because this would be too time consuming
                        queue.clear();
                        mp.queued_position = 0;
                    }
                    else
                    {
                        queue.push_back(MultiPass::get_input(mp));
                        ++mp.queued_position;
                    }
                    MultiPass::advance_input(mp);
                }
                else
                {
                    ++mp.queued_position;
                }
            }

            // called to forcibly clear the queue
            template <typename MultiPass>
            static void clear_queue(MultiPass& mp)
            {
                mp.shared()->queued_elements.clear();
                mp.queued_position = 0;
            }

            // called to determine whether the iterator is an eof iterator
            template <typename MultiPass>
            static bool is_eof(MultiPass const& mp)
            {
                return mp.queued_position ==
                    mp.shared()->queued_elements.size() &&
                    MultiPass::input_at_eof(mp);
            }

            // called by operator==
            template <typename MultiPass>
            static bool equal_to(MultiPass const& mp, MultiPass const& x)
            {
                return mp.queued_position == x.queued_position;
            }

            // called by operator<
            template <typename MultiPass>
            static bool less_than(MultiPass const& mp, MultiPass const& x)
            {
                return mp.queued_position < x.queued_position;
            }

            template <typename MultiPass>
            static void destroy(MultiPass&)
            {
            }

            mutable typename queue_type::size_type queued_position = 0;
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename Value>
        struct shared
        {
            shared()
            {
                queued_elements.reserve(threshold);
            }

            using queue_type = std::vector<Value>;
            queue_type queued_elements;
        };
    };    // split_std_deque
}    // namespace spirit::iterator_policies
