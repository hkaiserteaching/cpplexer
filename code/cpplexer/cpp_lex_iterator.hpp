/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Definition of the lexer iterator

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

#include <string>

#include "config.hpp"
#include "cpp_lex_interface_generator.hpp"
#include "file_position.hpp"
#include "language_support.hpp"
#include "multi_pass.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer {

    namespace impl {
        ///////////////////////////////////////////////////////////////////////////////
        //
        //  lex_iterator_functor_shim
        //
        ///////////////////////////////////////////////////////////////////////////////

        template <typename TokenT>
        class lex_iterator_functor_shim
        {
            using position_type = typename TokenT::position_type;

        public:
            lex_iterator_functor_shim() = default;

            // interface to the iterator_policies::split_functor_input policy
            using result_type = TokenT;
            using unique = lex_iterator_functor_shim;
            using shared = lex_input_interface<TokenT>*;

            result_type const eof;

            template <typename MultiPass>
            static result_type& get_next(MultiPass& mp, result_type& result)
            {
                return mp.shared()->ftor->get(result);
            }

            // this will be called whenever the last reference to a multi_pass will
            // be released
            template <typename MultiPass>
            static void destroy(MultiPass& mp)
            {
                delete mp.shared()->ftor;
            }

            template <typename MultiPass>
            static void set_position(MultiPass& mp, position_type const& pos)
            {
                mp.shared()->ftor->set_position(pos);
            }
        };

        ///////////////////////////////////////////////////////////////////////////////
    }    // namespace impl

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  lex_iterator
    //
    //      A generic C++ lexer interface class, which allows to plug in different
    //      lexer implementations. The interface between the lexer type used and
    //      the preprocessor component depends on the token type only (template
    //      parameter TokenT).
    //      Additionally, the following requirements apply:
    //
    //          - the lexer type should have a function implemented, which returns
    //            the next lexed token from the input stream:
    //                typename TokenT get();
    //          - at the end of the input stream this function should return the
    //            eof token equivalent
    //          - the lexer should implement a constructor taking two iterators
    //            pointing to the beginning and the end of the input stream,
    //            a third parameter containing the name of the parsed input file
    //            and a 4th parameter of the type wave::language_support
    //            which specifies, which language subset should be supported (C++,
    //            C99, C++11 etc.).
    //
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    //  Divide the given functor type into its components (unique and shared)
    //  and build a std::pair from these parts
    template <typename FunctorData>
    struct make_multi_pass
    {
        using functor_data_type = std::pair<typename FunctorData::unique,
            typename FunctorData::shared>;
        using result_type = typename FunctorData::result_type;

        using input_policy = spirit::iterator_policies::split_functor_input;
        using ownership_policy = spirit::iterator_policies::ref_counted;
#if defined(WAVE_DEBUG)
        typedef spirit::iterator_policies::buf_id_check check_policy;
#else
        using check_policy = spirit::iterator_policies::no_check;
#endif
        using storage_policy = spirit::iterator_policies::split_std_deque;

        using policy_type =
            spirit::iterator_policies::default_policy<ownership_policy,
                check_policy, input_policy, storage_policy>;
        using type = spirit::multi_pass<functor_data_type, policy_type>;
    };

    ///////////////////////////////////////////////////////////////////////////////
    template <typename TokenT>
    class lex_iterator
      : public make_multi_pass<impl::lex_iterator_functor_shim<TokenT>>::type
    {
        using input_policy_type = impl::lex_iterator_functor_shim<TokenT>;

        using base_type = typename make_multi_pass<input_policy_type>::type;
        using functor_data_type =
            typename make_multi_pass<input_policy_type>::functor_data_type;

        using unique_functor_type = typename input_policy_type::unique;
        using shared_functor_type = typename input_policy_type::shared;

    public:
        using token_type = TokenT;

        lex_iterator() = default;

        template <typename IteratorT>
        lex_iterator(IteratorT const& first, IteratorT const& last,
            typename TokenT::position_type const& pos,
            wave::language_support language)
          : base_type(functor_data_type(unique_functor_type(),
                lex_input_interface_generator<TokenT>::new_lexer(
                    first, last, pos, language)))
        {
        }

        void set_position(typename TokenT::position_type const& pos)
        {
            using position_type = typename TokenT::position_type;

            // set the new position in the current token
            token_type const& currtoken = this->base_type::dereference(*this);
            position_type currpos = currtoken.get_position();

            currpos.set_file(pos.get_file());
            currpos.set_line(pos.get_line());
            const_cast<token_type&>(currtoken).set_position(currpos);

            // set the new position for future tokens as well
            if (token_type::string_type::npos !=
                currtoken.get_value().find_first_of('\n'))
            {
                currpos.set_line(pos.get_line() + 1);
            }
            unique_functor_type::set_position(*this, currpos);
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
}    // namespace wave::cpplexer
