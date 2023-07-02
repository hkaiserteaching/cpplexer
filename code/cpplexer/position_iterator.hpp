/*=============================================================================
    Copyright (c) 2002 Juan Carlos Arevalo-Baeza
    Copyright (c) 2002-2006 Hartmut Kaiser
    Copyright (c) 2003 Giovanni Bajo
    http://spirit.sourceforge.net/

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include "position_iterator_fwd.hpp"

namespace spirit::classic {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  file_position_without_column
    //
    //  A structure to hold positional information. This includes the file,
    //  and the line number
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename String>
    struct file_position_without_column_base
    {
        String file;
        int line;

        file_position_without_column_base(
            String const& file_ = String(), int line_ = 1)
          : file(file_)
          , line(line_)
        {
        }

        friend bool operator==(file_position_without_column_base const& fpx,
            file_position_without_column_base const& fpy) noexcept
        {
            return fpx.line == fpy.line && fpx.file == fpy.file;
        }
        friend bool operator!=(file_position_without_column_base const& fpx,
            file_position_without_column_base const& fpy) noexcept
        {
            return !(fpx == fpy);
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  file_position
    //
    //  This structure holds complete file position, including file name,
    //  line and column number
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename String>
    struct file_position_base : file_position_without_column_base<String>
    {
        int column;

        explicit file_position_base(
            String const& file_ = String(), int line_ = 1, int column_ = 1)
          : file_position_without_column_base<String>(file_, line_)
          , column(column_)
        {
        }

        friend bool operator==(file_position_base const& fpx,
            file_position_base const& fpy) noexcept
        {
            return fpx.column == fpy.column && fpx.line == fpy.line &&
                fpx.file == fpy.file;
        }
        friend bool operator!=(file_position_base const& fpx,
            file_position_base const& fpy) noexcept
        {
            return !(fpx == fpy);
        }
    };

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  position_policy<>
    //
    //  This template is the policy to handle the file position. It is specialized
    //  on the position type. Providing a custom file_position also requires
    //  providing a specialization of this class.
    //
    //  Policy interface:
    //
    //    Default constructor of the custom position class must be accessible.
    //    set_tab_chars(unsigned int chars) - Set the tabstop width
    //    next_char(PositionT& pos)  - Notify that a new character has been
    //      processed
    //    tabulation(PositionT& pos) - Notify that a tab character has been
    //      processed
    //    next_line(PositionT& pos)  - Notify that a new line delimiter has
    //      been reached.
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename PositionT>
    class position_policy;
}    // namespace spirit::classic

// This must be included here for full compatibility with old MSVC
#include "detail/position_iterator.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace spirit::classic {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  position_iterator
    //
    //  It wraps an iterator, and keeps track of the current position in the input,
    //  as it gets incremented.
    //
    //  The wrapped iterator must be at least a Forward iterator. The position
    //  iterator itself will always be a non-mutable Forward iterator.
    //
    //  In order to have begin/end iterators constructed, the end iterator must be
    //  empty constructed. Similar to what happens with stream iterators. The begin
    //  iterator must be constructed from both, the begin and end iterators of the
    //  wrapped iterator type. This is necessary to implement the lookahead of
    //  characters necessary to parse CRLF sequences.
    //
    //  In order to extract the current positional data from the iterator, you may
    //  use the get_position member function.
    //
    //  You can also use the set_position member function to reset the current
    //  position to something new.
    //
    //  The structure that holds the current position can be customized through a
    //  template parameter, and the class position_policy must be specialized
    //  on the new type to define how to handle it. Currently, it's possible
    //  to choose between the file_position and file_position_without_column
    //  (which saves some overhead if managing current column is not required).
    //
    ///////////////////////////////////////////////////////////////////////////////

    template <typename ForwardIteratorT, typename PositionT, typename SelfT>
    class position_iterator
      : public iterator_::impl::position_iterator_base_generator<SelfT,
            ForwardIteratorT, PositionT>::type
      , public position_policy<PositionT>
    {
        using position_policy_t = position_policy<PositionT>;
        using base_t =
            typename iterator_::impl::position_iterator_base_generator<SelfT,
                ForwardIteratorT, PositionT>::type;
        using main_iter_t =
            typename iterator_::impl::position_iterator_base_generator<SelfT,
                ForwardIteratorT, PositionT>::main_iter_t;

    public:
        using position_t = PositionT;

        position_iterator()
          : _isend(true)
        {
        }

        position_iterator(
            ForwardIteratorT const& begin, ForwardIteratorT const& end)
          : base_t(begin)
          , _end(end)
          , _pos(PositionT())
          , _isend(begin == end)
        {
        }

        template <typename FileNameT>
        position_iterator(ForwardIteratorT const& begin,
            ForwardIteratorT const& end, FileNameT fileName)
          : base_t(begin)
          , _end(end)
          , _pos(PositionT(fileName))
          , _isend(begin == end)
        {
        }

        template <typename FileNameT, typename LineT>
        position_iterator(ForwardIteratorT const& begin,
            ForwardIteratorT const& end, FileNameT fileName, LineT line)
          : base_t(begin)
          , _end(end)
          , _pos(PositionT(fileName, line))
          , _isend(begin == end)
        {
        }

        template <typename FileNameT, typename LineT, typename ColumnT>
        position_iterator(ForwardIteratorT const& begin,
            ForwardIteratorT const& end, FileNameT fileName, LineT line,
            ColumnT column)
          : base_t(begin)
          , _end(end)
          , _pos(PositionT(fileName, line, column))
          , _isend(begin == end)
        {
        }

        position_iterator(ForwardIteratorT const& begin,
            ForwardIteratorT const& end, PositionT const& pos)
          : base_t(begin)
          , _end(end)
          , _pos(pos)
          , _isend(begin == end)
        {
        }

        position_iterator(position_iterator const& iter)
          : base_t(iter.base())
          , position_policy_t(iter)
          , _end(iter._end)
          , _pos(iter._pos)
          , _isend(iter._isend)
        {
        }

        position_iterator& operator=(position_iterator const& iter)
        {
            base_t::operator=(iter);
            position_policy_t::operator=(iter);
            _end = iter._end;
            _pos = iter._pos;
            _isend = iter._isend;
            return *this;
        }

        void set_position(PositionT const& newpos)
        {
            _pos = newpos;
        }
        PositionT& get_position()
        {
            return _pos;
        }
        PositionT const& get_position() const
        {
            return _pos;
        }

        void set_tabchars(unsigned int chars)
        {
            // This function (which comes from the position_policy) has a
            //  different name on purpose, to avoid messing with using
            //  declarations or qualified calls to access the base template
            //  function, which might break some compilers.
            this->position_policy_t::set_tab_chars(chars);
        }

    private:
        friend class iterator_core_access;

        void increment()
        {
            typename base_t::reference val = *(this->base());
            if (val == '\n')
            {
                ++this->base_reference();
                this->next_line(_pos);
                static_cast<main_iter_t&>(*this).newline();
            }
            else if (val == '\r')
            {
                ++this->base_reference();
                if (this->base_reference() == _end || *(this->base()) != '\n')
                {
                    this->next_line(_pos);
                    static_cast<main_iter_t&>(*this).newline();
                }
            }
            else if (val == '\t')
            {
                this->tabulation(_pos);
                ++this->base_reference();
            }
            else
            {
                this->next_char(_pos);
                ++this->base_reference();
            }

            // The iterator is at the end only if it's the same
            //  of the
            _isend = (this->base_reference() == _end);
        }

        template <typename OtherDerivedT, typename OtherIteratorT, typename V,
            typename C, typename R, typename D>
        bool equal(hpx::iterator_adaptor<OtherDerivedT, OtherIteratorT, V, C, R,
            D> const& x) const
        {
            OtherDerivedT const& rhs = static_cast<OtherDerivedT const&>(x);
            bool const x_is_end = rhs._isend;

            return (_isend == x_is_end) &&
                (_isend || this->base() == rhs.base());
        }

    protected:
        void newline() {}

        ForwardIteratorT _end;
        PositionT _pos;
        bool _isend;
    };

}    // namespace spirit::classic
