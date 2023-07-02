//  Copyright (c) 2016 Thomas Heller
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace hpx {

    struct identity
    {
        using is_transparent = std::true_type;

        template <typename T>
         constexpr T&& operator()(T&& t) const noexcept
        {
            return std::forward<T>(t);
        }
    };

    inline constexpr identity identity_v = identity();

    template <typename T>
    struct type_identity
    {
        using type = T;
    };

    template <typename T>
    using type_identity_t = typename type_identity<T>::type;

}    // namespace hpx
