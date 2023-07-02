/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    A generic C++ lexer token definition

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include "config.hpp"
#include "file_position.hpp"
#include "token_ids.hpp"

#include <cassert>
#include <cstddef>
#include <optional>

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer {

    namespace impl {

        template <typename StringTypeT, typename PositionT>
        class token_data
        {
        public:
            using string_type = StringTypeT;
            using position_type = PositionT;

            //  default constructed tokens correspond to EOI tokens
            token_data() noexcept
              : id(T_EOI)
              , refcnt(1)
            {
            }

            //  construct an invalid token
            explicit token_data(int) noexcept
              : id(T_UNKNOWN)
              , refcnt(1)
            {
            }

            token_data(token_id id_, string_type value_, position_type pos_,
                std::optional<position_type> expand_pos_ = std::nullopt)
              : id(id_)
              , value(std::move(value_))
              , pos(std::move(pos_))
              , expand_pos(std::move(expand_pos_))
              , refcnt(1)
            {
            }

            token_data(token_data const& rhs)
              : id(rhs.id)
              , value(rhs.value)
              , pos(rhs.pos)
              , expand_pos(rhs.expand_pos)
              , refcnt(1)
            {
            }

            ~token_data() = default;

            std::size_t addref() noexcept
            {
                return ++refcnt;
            }
            std::size_t release() noexcept
            {
                return --refcnt;
            }

            [[nodiscard]] std::size_t get_refcnt() const noexcept
            {
                return refcnt;
            }

            // accessors
            operator token_id() const noexcept
            {
                return id;
            }

            [[nodiscard]] string_type const& get_value() const noexcept
            {
                return value;
            }

            [[nodiscard]] position_type const& get_position() const noexcept
            {
                return pos;
            }

            [[nodiscard]] position_type const& get_expand_position()
                const noexcept
            {
                if (expand_pos)
                    return *expand_pos;
                return pos;
            }

            void set_token_id(token_id id_) noexcept
            {
                id = id_;
            }
            void set_value(string_type const& value_)
            {
                value = value_;
            }
            void set_position(position_type const& pos_)
            {
                pos = pos_;
            }
            void set_expand_position(position_type const& pos_)
            {
                expand_pos = pos_;
            }

            friend bool operator==(
                token_data const& lhs, token_data const& rhs) noexcept
            {
                //  two tokens are considered equal even if they refer to different
                //  positions
                return lhs.id == rhs.id && lhs.value == rhs.value;
            }
            friend bool operator!=(
                token_data const& lhs, token_data const& rhs) noexcept
            {
                //  two tokens are considered equal even if they refer to different
                //  positions
                return !(lhs == rhs);
            }

            void init(token_id id_, string_type const& value_,
                position_type const& pos_)
            {
                assert(refcnt == 1);
                id = id_;
                value = value_;
                pos = pos_;
            }

            void init(token_data const& rhs)
            {
                assert(refcnt == 1);
                id = rhs.id;
                value = rhs.value;
                pos = rhs.pos;
            }

            void* operator new(std::size_t size);
            void operator delete(void* p);
            void operator delete(void* p, std::size_t size);

        private:
            token_id id;          // the token id
            string_type value;    // the text that was parsed into this token
            position_type pos;    // the original file position
            // where was this token expanded
            std::optional<position_type> expand_pos;
            std::size_t refcnt;
        };

        ///////////////////////////////////////////////////////////////////////////////
        struct token_data_tag
        {
        };

        template <typename StringTypeT, typename PositionT>
        void* token_data<StringTypeT, PositionT>::operator new(std::size_t size)
        {
            return std::malloc(sizeof(token_data<StringTypeT, PositionT>));
        }

        template <typename StringTypeT, typename PositionT>
        void token_data<StringTypeT, PositionT>::operator delete(
            void* p, std::size_t)
        {
            if (nullptr != p)
                std::free(p);
        }
        template <typename StringTypeT, typename PositionT>
        void token_data<StringTypeT, PositionT>::operator delete(void* p)
        {
            if (nullptr != p)
                std::free(p);
        }

    }    // namespace impl

    ///////////////////////////////////////////////////////////////////////////////
    //  forward declaration of the token type
    template <typename PositionT = util::file_position_type>
    class lex_token;

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  lex_token
    //
    ///////////////////////////////////////////////////////////////////////////////

    template <typename PositionT>
    class lex_token
    {
    public:
        using string_type = WAVE_STRINGTYPE;
        using position_type = PositionT;

    private:
        using data_type = impl::token_data<string_type, position_type>;

    public:
        //  default constructed tokens correspond to EOI tokens
        lex_token() = default;

        //  construct an invalid token
        explicit lex_token(int)
          : data(new data_type(0))
        {
        }

        lex_token(lex_token const& rhs)
          : data(rhs.data)
        {
            if (nullptr != data)
                data->addref();
        }

        lex_token(
            token_id id_, string_type const& value_, PositionT const& pos_)
          : data(new data_type(id_, value_, pos_))
        {
        }

        ~lex_token()
        {
            if (nullptr != data && 0 == data->release())
                delete data;
            data = nullptr;
        }

        lex_token& operator=(lex_token const& rhs)
        {
            if (&rhs != this)
            {
                if (nullptr != data && 0 == data->release())
                    delete data;

                data = rhs.data;
                if (nullptr != data)
                    data->addref();
            }
            return *this;
        }

        // accessors
        explicit operator token_id() const noexcept
        {
            return nullptr != data ? static_cast<token_id>(*data) : T_EOI;
        }

        [[nodiscard]] string_type const& get_value() const noexcept
        {
            return data->get_value();
        }

        [[nodiscard]] position_type const& get_position() const noexcept
        {
            return data->get_position();
        }

        [[nodiscard]] position_type const& get_expand_position() const noexcept
        {
            return data->get_expand_position();
        }

        [[nodiscard]] bool is_eoi() const noexcept
        {
            return nullptr == data || static_cast<token_id>(*data) == T_EOI;
        }

        [[nodiscard]] bool is_valid() const noexcept
        {
            return nullptr != data && static_cast<token_id>(*data) != T_UNKNOWN;
        }

        void set_token_id(token_id id_)
        {
            make_unique();
            data->set_token_id(id_);
        }
        void set_value(string_type const& value_)
        {
            make_unique();
            data->set_value(value_);
        }
        void set_position(position_type const& pos_)
        {
            make_unique();
            data->set_position(pos_);
        }
        void set_expand_position(position_type const& pos_)
        {
            make_unique();
            data->set_expand_position(pos_);
        }

        friend bool operator==(
            lex_token const& lhs, lex_token const& rhs) noexcept
        {
            if (nullptr == rhs.data)
                return nullptr == lhs.data;
            if (nullptr == lhs.data)
                return false;
            return *(lhs.data) == *(rhs.data);
        }
        friend bool operator!=(
            lex_token const& lhs, lex_token const& rhs) noexcept
        {
            return !(lhs == rhs);
        }

    private:
        // make a unique copy of the current object
        void make_unique()
        {
            if (1 == data->get_refcnt())
                return;

            data_type* newdata = new data_type(*data);

            data->release();    // release this reference, can't get zero
            data = newdata;
        }

        data_type* data = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////////
    //  This overload is needed by the multi_pass/functor_input_policy to
    //  validate a token instance. It has to be defined in the same namespace
    //  as the token class itself to allow ADL to find it.
    ///////////////////////////////////////////////////////////////////////////////
    template <typename Position>
    bool token_is_valid(lex_token<Position> const& t)
    {
        return t.is_valid();
    }

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer
