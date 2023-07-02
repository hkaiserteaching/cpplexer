//  Copyright (c) 2001 Daniel C. Nuffer
//  Copyright (c) 2001-2011 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

///////////////////////////////////////////////////////////////////////////////
namespace spirit::detail {

    ///////////////////////////////////////////////////////////////////////////
    //  Default implementations of the different policies to be used with a
    //  multi_pass iterator
    ///////////////////////////////////////////////////////////////////////////
    struct default_input_policy
    {
        default_input_policy() = default;

        template <typename Functor>
        explicit constexpr default_input_policy(Functor const&) noexcept
        {
        }

        template <typename MultiPass>
        static constexpr void destroy(MultiPass&) noexcept
        {
        }

        constexpr void swap(default_input_policy&) noexcept {}

        template <typename MultiPass, typename TokenType>
        static void advance_input(MultiPass& mp);

        template <typename MultiPass>
        static typename MultiPass::reference get_input(MultiPass& mp);

        template <typename MultiPass>
        static bool input_at_eof(MultiPass const& mp);

        template <typename MultiPass, typename TokenType>
        static bool input_is_valid(MultiPass& mp, TokenType& curtok);
    };

    struct default_ownership_policy
    {
        template <typename MultiPass>
        static constexpr void destroy(MultiPass&) noexcept
        {
        }

        constexpr void swap(default_ownership_policy&) noexcept {}

        template <typename MultiPass>
        static constexpr void clone(MultiPass&) noexcept
        {
        }

        template <typename MultiPass>
        static bool release(MultiPass& mp);

        template <typename MultiPass>
        static bool is_unique(MultiPass const& mp);
    };

    struct default_storage_policy
    {
        template <typename MultiPass>
        static constexpr void destroy(MultiPass&) noexcept
        {
        }

        constexpr void swap(default_storage_policy&) noexcept {}

        template <typename MultiPass>
        static typename MultiPass::reference dereference(MultiPass const& mp);

        template <typename MultiPass>
        static constexpr void increment(MultiPass&) noexcept
        {
        }

        template <typename MultiPass>
        static constexpr void clear_queue(MultiPass&) noexcept
        {
        }

        template <typename MultiPass>
        static bool is_eof(MultiPass const& mp);

        template <typename MultiPass>
        static bool equal_to(MultiPass const& mp, MultiPass const& x);

        template <typename MultiPass>
        static bool less_than(MultiPass const& mp, MultiPass const& x);
    };

    struct default_checking_policy
    {
        template <typename MultiPass>
        static constexpr void destroy(MultiPass&) noexcept
        {
        }

        constexpr void swap(default_checking_policy&) noexcept {}

        template <typename MultiPass>
        static constexpr void docheck(MultiPass const&) noexcept
        {
        }

        template <typename MultiPass>
        static constexpr void clear_queue(MultiPass&) noexcept
        {
        }
    };
}    // namespace spirit::detail
