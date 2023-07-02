//  Copyright (c) 2001, Daniel C. Nuffer
//  Copyright (c) 2001-2011 Hartmut Kaiser
//  http://spirit.sourceforge.net/
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "multi_pass_fwd.hpp"

#include "detail/combine_policies.hpp"
#include "detail/multi_pass.hpp"
#include "detail/no_check_policy.hpp"
#include "detail/ref_counted_policy.hpp"
#include "detail/split_functor_input_policy.hpp"
#include "detail/split_std_deque_policy.hpp"

#include <limits>
#include <utility>

namespace spirit {

    template <typename MemberType, int UniqueID = 0>
    class base_from_member
    {
    protected:
        MemberType member;

        template <typename... T>
        explicit constexpr base_from_member(T&&... x) noexcept(noexcept(::new(
            static_cast<void*>(nullptr)) MemberType(static_cast<T&&>(x)...)))
          : member(static_cast<T&&>(x)...)
        {
        }
    };    // boost::base_from_member

    ///////////////////////////////////////////////////////////////////////////
    // The default multi_pass instantiation uses a ref-counted std_deque scheme.
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Policies>
    class multi_pass
      : base_from_member<typename Policies::template shared<T>*>
      , public Policies::template unique<T>
    {
    private:
        // unique and shared data types
        using policies_base_type = typename Policies::template unique<T>;
        using shared_data_type = typename Policies::template shared<T>;

        using member_base = base_from_member<shared_data_type*>;

        // define the types the standard embedded iterator typedefs are taken
        // from
        using iterator_type = typename policies_base_type::input_policy;

    public:
        // standard iterator typedefs
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename iterator_type::value_type;
        using difference_type = typename iterator_type::difference_type;
        using distance_type = typename iterator_type::distance_type;
        using reference = typename iterator_type::reference;
        using pointer = typename iterator_type::pointer;

        multi_pass()
          : member_base(static_cast<shared_data_type*>(nullptr))
        {
        }

        explicit multi_pass(T& input)
          : member_base(new shared_data_type(input))
          , policies_base_type(input)
        {
        }

        explicit multi_pass(T const& input)
          : member_base(new shared_data_type(input))
          , policies_base_type(input)
        {
        }

        multi_pass(multi_pass const& x)
          : member_base(x.member)
          , policies_base_type(x)
        {
            policies_base_type::clone(*this);
        }

        ~multi_pass()
        {
            if (policies_base_type::release(*this))
            {
                policies_base_type::destroy(*this);
                delete this->member;
            }
        }

        multi_pass& operator=(multi_pass const& x)
        {
            if (this != &x)
            {
                multi_pass temp(x);
                temp.swap(*this);
            }
            return *this;
        }

        void swap(multi_pass& x) noexcept
        {
            using std::swap;
            swap(this->member, x.member);
            this->policies_base_type::swap(x);
        }

        reference operator*() const
        {
            policies_base_type::docheck(*this);
            return policies_base_type::dereference(*this);
        }
        pointer operator->() const
        {
            return &(operator*());
        }

        multi_pass& operator++()
        {
            policies_base_type::docheck(*this);
            policies_base_type::increment(*this);
            return *this;
        }
        multi_pass operator++(int)
        {
            multi_pass tmp(*this);
            ++*this;
            return tmp;
        }

        void clear_queue(
            traits::clear_mode mode = traits::clear_mode::clear_if_enabled)
        {
            if (mode == traits::clear_mode::clear_always ||
                !inhibit_clear_queue())
                policies_base_type::clear_queue(*this);
        }

        [[nodiscard]] bool inhibit_clear_queue() const
        {
            return this->member->inhibit_clear_queue_;
        }
        void inhibit_clear_queue(bool flag)
        {
            this->member->inhibit_clear_queue_ = flag;
        }

        friend bool operator==(multi_pass const& x, multi_pass const& y)
        {
            if (x.is_eof())
                return y.is_eof();
            if (y.is_eof())
                return false;

            return policies_base_type::equal_to(x, y);
        }
        friend bool operator<(multi_pass const& x, multi_pass const& y)
        {
            return policies_base_type::less_than(x, y);
        }

        friend bool operator!=(multi_pass const& x, multi_pass const& y)
        {
            return !(x == y);
        }
        friend bool operator>(multi_pass const& x, multi_pass const& y)
        {
            return y < x;
        }
        friend bool operator>=(multi_pass const& x, multi_pass const& y)
        {
            return !(x < y);
        }
        friend bool operator<=(multi_pass const& x, multi_pass const& y)
        {
            return !(y < x);
        }

        // allow access to base member
        shared_data_type* shared() const
        {
            return this->member;
        }

    private:    // helper functions
        [[nodiscard]] bool is_eof() const
        {
            return (0 == this->member) || policies_base_type::is_eof(*this);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    //  Generator function
    ///////////////////////////////////////////////////////////////////////////
    template <typename Policies, typename T>
    multi_pass<T, Policies> make_multi_pass(T& i)
    {
        return multi_pass<T, Policies>(i);
    }
    template <typename Policies, typename T>
    multi_pass<T, Policies> make_multi_pass(T const& i)
    {
        return multi_pass<T, Policies>(i);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    multi_pass<T> make_default_multi_pass(T& i)
    {
        return multi_pass<T>(i);
    }
    template <typename T>
    multi_pass<T> make_default_multi_pass(T const& i)
    {
        return multi_pass<T>(i);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Policies>
    void swap(multi_pass<T, Policies>& x, multi_pass<T, Policies>& y) noexcept
    {
        x.swap(y);
    }

    ///////////////////////////////////////////////////////////////////////////
    // define special functions allowing to integrate any multi_pass iterator
    // with expectation points
    namespace traits {

        template <typename T, typename Policies>
        void clear_queue(multi_pass<T, Policies>& mp, traits::clear_mode mode)
        {
            mp.clear_queue(mode);
        }

        template <typename T, typename Policies>
        void inhibit_clear_queue(multi_pass<T, Policies>& mp, bool flag)
        {
            mp.inhibit_clear_queue(flag);
        }

        template <typename T, typename Policies>
        bool inhibit_clear_queue(multi_pass<T, Policies>& mp)
        {
            return mp.inhibit_clear_queue();
        }
    }    // namespace traits
}    // namespace spirit
