/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001 Daniel C. Nuffer.
    Copyright (c) 2001-2012 Hartmut Kaiser.
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include "../config.hpp"

#include <cstdlib>

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer::re2clex {
    using aq_stdelement = std::size_t;

    using aq_queuetype = struct tag_aq_queuetype
    {
        std::size_t head;
        std::size_t tail;
        std::size_t size;
        std::size_t max_size;
        aq_stdelement* queue;
    };

    using aq_queue = aq_queuetype*;

    WAVE_DECL int aq_enqueue(aq_queue q, aq_stdelement e);
    int aq_enqueue_front(aq_queue q, aq_stdelement e);
    int aq_serve(aq_queue q, aq_stdelement* e);
    WAVE_DECL int aq_pop(aq_queue q);

#define AQ_EMPTY(q) ((q)->size == 0)
#define AQ_FULL(q) ((q)->size == q->max_size)

    int aq_grow(aq_queue q);

    WAVE_DECL aq_queue aq_create();
    WAVE_DECL void aq_terminate(aq_queue q);

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer::re2clex
