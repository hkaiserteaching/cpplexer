/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include <vector>

#include "config.hpp"
#include "token_ids.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  The token_cache template is used to cache the tokens corresponding to the
    //  keywords, operators and other constant language elements.
    //
    //  This avoids repeated construction of these tokens, which is especially
    //  effective when used in conjunction with a copy on write string
    //  implementation (COW string).
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename StringT>
    class token_cache
    {
    public:
        token_cache()
          : cache(T_LAST_TOKEN - T_FIRST_TOKEN)
        {
            auto it = cache.begin();
            for (unsigned int i = T_FIRST_TOKEN; i < T_LAST_TOKEN; ++i, ++it)
            {
                *it = StringT(get_token_value(static_cast<token_id>(i)));
            }
        }

        [[nodiscard]] StringT const& get_token_value(token_id const id) const
        {
            return cache[BASEID_FROM_TOKEN(id) - T_FIRST_TOKEN];
        }

    private:
        std::vector<StringT> cache;
    };

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer
