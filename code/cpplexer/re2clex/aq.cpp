/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001 Daniel C. Nuffer.
    Copyright (c) 2001-2012 Hartmut Kaiser.
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <cassert>
#include <cstdlib>
#include <cstring>

#include "../config.hpp"    // configuration data
#include "aq.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer::re2clex {

    int aq_grow(aq_queue q)
    {
        using namespace std;    // some systems have memcpy/realloc in std
        const std::size_t new_size = q->max_size << 1;
        auto* new_queue = static_cast<aq_stdelement*>(
            realloc(q->queue, new_size * sizeof(aq_stdelement)));

        assert(NULL != q);
        assert(q->max_size < 100000);
        assert(q->size <= q->max_size);

#define ASSERT_SIZE                                                            \
    assert(((q->tail + q->max_size + 1) - q->head) % q->max_size ==            \
        q->size % q->max_size)

        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        if (!new_queue)
        {
            assert(0);
            return 0;
        }

        q->queue = new_queue;
        if (q->tail <= q->head) /* tail has wrapped around */
        {
            /* move the tail from the beginning to the end */
            memcpy(q->queue + q->max_size, q->queue,
                (q->tail + 1) * sizeof(aq_stdelement));
            q->tail += q->max_size;
        }
        q->max_size = new_size;

        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        return 1;
    }

    int aq_enqueue(aq_queue q, aq_stdelement e)
    {
        assert(NULL != q);
        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        if (AQ_FULL(q))
            if (!aq_grow(q))
                return 0;

        ++q->tail;
        if (q->tail == q->max_size)
            q->tail = 0;

        q->queue[q->tail] = e;
        ++q->size;

        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        return 1;
    }

    int aq_enqueue_front(aq_queue q, aq_stdelement e)
    {
        assert(NULL != q);

        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        if (AQ_FULL(q))
            if (!aq_grow(q))
                return 0;

        if (q->head == 0)
            q->head = q->max_size - 1;
        else
            --q->head;

        q->queue[q->head] = e;
        ++q->size;

        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        return 1;
    }

    int aq_serve(aq_queue q, aq_stdelement* e)
    {
        assert(NULL != q);
        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        if (AQ_EMPTY(q))
            return 0;

        *e = q->queue[q->head];
        return aq_pop(q);
    }

    int aq_pop(aq_queue q)
    {
        assert(NULL != q);
        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        if (AQ_EMPTY(q))
            return 0;

        ++q->head;
        if (q->head == q->max_size)
            q->head = 0;
        --q->size;

        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        return 1;
    }

    aq_queue aq_create(void)
    {
        using namespace std;    // some systems have malloc in std
        const auto q = static_cast<aq_queue>(malloc(sizeof(aq_queuetype)));
        if (!q)
        {
            return nullptr;
        }

        q->max_size = 8; /* initial size */
        q->queue = static_cast<aq_stdelement*>(
            malloc(sizeof(aq_stdelement) * q->max_size));
        if (!q->queue)
        {
            free(q);
            return nullptr;
        }

        q->head = 0;
        q->tail = q->max_size - 1;
        q->size = 0;

        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        return q;
    }

    void aq_terminate(aq_queue q)
    {
        using namespace std;    // some systems have free in std

        assert(NULL != q);
        assert(q->size <= q->max_size);
        ASSERT_SIZE;
        assert(q->head <= q->max_size);
        assert(q->tail <= q->max_size);

        free(q->queue);
        free(q);
    }

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer::re2clex
