//  Copyright (c) 2001 Daniel C. Nuffer
//  Copyright (c) 2001-2011 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../multi_pass_fwd.hpp"

#include "multi_pass.hpp"

namespace spirit::iterator_policies {

    ///////////////////////////////////////////////////////////////////////////
    //  class no_check
    //  Implementation of the CheckingPolicy used by multi_pass
    //  It does not do anything :-)
    ///////////////////////////////////////////////////////////////////////////
    struct no_check
    {
        ///////////////////////////////////////////////////////////////////////
        struct unique : detail::default_checking_policy
        {
        };

        ///////////////////////////////////////////////////////////////////////
        struct shared
        {
        };
    };
}    // namespace spirit::iterator_policies
