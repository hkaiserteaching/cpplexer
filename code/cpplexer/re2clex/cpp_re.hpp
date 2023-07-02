/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Re2C based C++ lexer

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include <cassert>

#include "../config.hpp"
#include "../cpplexer_exceptions.hpp"
#include "../token_ids.hpp"
#include "aq.hpp"
#include "scanner.hpp"

///////////////////////////////////////////////////////////////////////////////
#define YYCTYPE uchar
#define YYCURSOR cursor
#define YYLIMIT limit
#define YYMARKER marker
#define YYFILL(n)                                                              \
    {                                                                          \
        cursor = uchar_wrapper(fill(s, cursor), cursor.column);                \
        limit = uchar_wrapper(s->lim);                                         \
    }                                                                          \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define WAVE_UPDATE_CURSOR()                                                   \
    {                                                                          \
        s->line += count_backslash_newlines(s, cursor);                        \
        s->curr_column = cursor.column;                                        \
        s->cur = cursor;                                                       \
        s->lim = limit;                                                        \
        s->ptr = marker;                                                       \
    }                                                                          \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define WAVE_RET(i)                                                            \
    {                                                                          \
        WAVE_UPDATE_CURSOR()                                                   \
        if (s->cur > s->lim)                                                   \
            return T_EOF; /* may happen for empty files */                     \
        return (i);                                                            \
    }                                                                          \
    /**/

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer::re2clex {

    template <typename Iterator>
    struct Scanner;

    ///////////////////////////////////////////////////////////////////////////////
    //  The scanner function to call whenever a new token is requested
    template <typename Iterator>
    token_id scan(Scanner<Iterator>* s);
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    //  Utility functions

#define RE2C_assert assert

    template <typename Iterator>
    int get_one_char(Scanner<Iterator>* s)
    {
        RE2C_assert(s->first <= s->act && s->act <= s->last);
        if (s->act < s->last)
            return *(s->act)++;
        return -1;
    }

    template <typename Iterator>
    std::ptrdiff_t rewind_stream(Scanner<Iterator>* s, int cnt)
    {
        std::advance(s->act, cnt);
        RE2C_assert(s->first <= s->act && s->act <= s->last);
        return std::distance(s->first, s->act);
    }

    template <typename Iterator>
    std::size_t get_first_eol_offset(Scanner<Iterator>* s)
    {
        if (!AQ_EMPTY(s->eol_offsets))
        {
            return s->eol_offsets->queue[s->eol_offsets->head];
        }
        return static_cast<unsigned int>(-1);
    }

    template <typename Iterator>
    void adjust_eol_offsets(Scanner<Iterator>* s, std::size_t adjustment)
    {
        if (!s->eol_offsets)
            s->eol_offsets = aq_create();

        aq_queue const q = s->eol_offsets;

        if (AQ_EMPTY(q))
            return;

        std::size_t i = q->head;
        while (i != q->tail)
        {
            if (adjustment > q->queue[i])
                q->queue[i] = 0;
            else
                q->queue[i] -= adjustment;
            ++i;
            if (i == q->max_size)
                i = 0;
        }
        if (adjustment > q->queue[i])
            q->queue[i] = 0;
        else
            q->queue[i] -= adjustment;
    }

    template <typename Iterator>
    int count_backslash_newlines(Scanner<Iterator>* s, uchar* cursor)
    {
        int skipped = 0;

        /* figure out how many backslash-newlines skipped over unknowingly. */
        std::size_t const diff = cursor - s->bot;
        std::size_t offset = get_first_eol_offset(s);
        while (offset <= diff && offset != static_cast<unsigned int>(-1))
        {
            skipped++;
            aq_pop(s->eol_offsets);
            offset = get_first_eol_offset(s);
        }
        return skipped;
    }

    bool is_backslash(const uchar* p, const uchar* end, int& len);

#define WAVE_BSIZE 196608
    template <typename Iterator>
    uchar* fill(Scanner<Iterator>* s, uchar* cursor)
    {
        using namespace std;    // some systems have memcpy etc. in namespace std
        if (!s->eof)
        {
            std::ptrdiff_t cnt = s->tok - s->bot;
            if (cnt)
            {
                if (NULL == s->lim)
                    s->lim = s->top;
                if (size_t const length = s->lim - s->tok; length > 0)
                {
                    memmove(s->bot, s->tok, length);
                }
                s->tok = s->cur = s->bot;
                s->ptr -= cnt;
                cursor -= cnt;
                s->lim -= cnt;
                adjust_eol_offsets(s, cnt);
            }

            if ((s->top - s->lim) < WAVE_BSIZE)
            {
                auto* buf = static_cast<uchar*>(
                    malloc(((s->lim - s->bot) + WAVE_BSIZE) * sizeof(uchar)));
                if (buf == nullptr)
                {
                    (*s->error_proc)(s, lexing_exception::unexpected_error,
                        "Out of memory!");

                    /* get the scanner to stop */
                    *cursor = 0;
                    return cursor;
                }

                if (size_t const length = s->lim - s->tok; length > 0)
                {
                    memmove(buf, s->tok, length);
                }
                s->tok = s->cur = buf;
                s->ptr = &buf[s->ptr - s->bot];
                cursor = &buf[cursor - s->bot];
                s->lim = &buf[s->lim - s->bot];
                s->top = &s->lim[WAVE_BSIZE];
                free(s->bot);
                s->bot = buf;
            }

            cnt = std::distance(s->act, s->last);
            if (cnt > WAVE_BSIZE)
                cnt = WAVE_BSIZE;
            uchar* dst = s->lim;
            for (std::ptrdiff_t idx = 0; idx < cnt; ++idx)
            {
                *dst++ = *s->act++;
            }

            if (cnt != WAVE_BSIZE)
            {
                s->eof = &s->lim[cnt];
                *(s->eof)++ = '\0';
            }

            /* backslash-newline erasing time */

            /* first scan for backslash-newline and erase them */
            for (uchar* p = s->lim; p < s->lim + cnt - 2; ++p)
            {
                if (int len = 0; is_backslash(p, s->lim + cnt, len))
                {
                    if (*(p + len) == '\n')
                    {
                        int offset = len + 1;
                        memmove(p, p + offset, s->lim + cnt - p - offset);
                        cnt -= offset;
                        --p;
                        aq_enqueue(s->eol_offsets, p - s->bot + 1);
                    }
                    else if (*(p + len) == '\r')
                    {
                        if (*(p + len + 1) == '\n')
                        {
                            int offset = len + 2;
                            memmove(p, p + offset, s->lim + cnt - p - offset);
                            cnt -= offset;
                            --p;
                        }
                        else
                        {
                            int offset = len + 1;
                            memmove(p, p + offset, s->lim + cnt - p - offset);
                            cnt -= offset;
                            --p;
                        }
                        aq_enqueue(s->eol_offsets, p - s->bot + 1);
                    }
                }
            }

            /* FIXME: the following code should be fixed to recognize correctly the
                  trigraph backslash token */

            /* check to see if what we just read ends in a backslash */
            if (cnt >= 2)
            {
                uchar const last = s->lim[cnt - 1];
                uchar const last2 = s->lim[cnt - 2];
                /* check \ EOB */
                if (last == '\\')
                {
                    int const next = get_one_char(s);
                    /* check for \ \n or \ \r or \ \r \n straddling the border */
                    if (next == '\n')
                    {
                        --cnt; /* chop the final \, we've already read the \n. */
                        aq_enqueue(s->eol_offsets, cnt + (s->lim - s->bot));
                    }
                    else if (next == '\r')
                    {
                        int const next2 = get_one_char(s);
                        if (next2 == '\n')
                        {
                            --cnt; /* skip the backslash */
                        }
                        else
                        {
                            /* rewind one, and skip one char */
                            rewind_stream(s, -1);
                            --cnt;
                        }
                        aq_enqueue(s->eol_offsets, cnt + (s->lim - s->bot));
                    }
                    else if (next != -1) /* -1 means end of file */
                    {
                        /* next was something else, so rewind the stream */
                        rewind_stream(s, -1);
                    }
                }
                /* check \ \r EOB */
                else if (last == '\r' && last2 == '\\')
                {
                    int const next = get_one_char(s);
                    if (next == '\n')
                    {
                        cnt -= 2; /* skip the \ \r */
                    }
                    else
                    {
                        /* rewind one, and skip two chars */
                        rewind_stream(s, -1);
                        cnt -= 2;
                    }
                    aq_enqueue(s->eol_offsets, cnt + (s->lim - s->bot));
                }
                /* check \ \n EOB */
                else if (last == '\n' && last2 == '\\')
                {
                    cnt -= 2;
                    aq_enqueue(s->eol_offsets, cnt + (s->lim - s->bot));
                }
            }

            s->lim += cnt;
            if (s->eof) /* eof needs adjusting if we erased backslash-newlines */
            {
                s->eof = s->lim;
                *(s->eof)++ = '\0';
            }
        }
        return cursor;
    }
#undef WAVE_BSIZE

    ///////////////////////////////////////////////////////////////////////////////
    //  Special wrapper class holding the current cursor position
    struct WAVE_DECL uchar_wrapper
    {
        uchar_wrapper(uchar* base_cursor, std::size_t column = 1);

        uchar_wrapper& operator++();

        uchar_wrapper& operator--();

        uchar operator*() const;

        operator uchar*() const;

        friend WAVE_DECL std::ptrdiff_t operator-(
            uchar_wrapper const& lhs, uchar_wrapper const& rhs);

        uchar* base_cursor;
        std::size_t column;
    };

    ///////////////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    token_id scan(Scanner<Iterator>* s)
    {
        assert(0 != s->error_proc);    // error handler must be given

        uchar_wrapper cursor(s->tok = s->cur, s->column = s->curr_column);
        uchar_wrapper marker(s->ptr);
        uchar_wrapper limit(s->lim);

        using string_type = WAVE_STRINGTYPE;
        string_type rawstringdelim;    // for use with C++11 raw string literals

// include the correct Re2C token definition rules
#if (defined(__FreeBSD__) || defined(__DragonFly__) ||                         \
    defined(__OpenBSD__)) &&                                                   \
    defined(T_DIVIDE)
#undef T_DIVIDE
#endif

#include "strict_cpp_re.hpp"

    } /* end of scan */

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer::re2clex

#undef WAVE_RET
#undef YYCTYPE
#undef YYCURSOR
#undef YYLIMIT
#undef YYMARKER
#undef YYFILL
