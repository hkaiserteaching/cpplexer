/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Grammar for universal character validation (see C++ standard: Annex E)

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

///////////////////////////////////////////////////////////////////////////////
namespace wave::cpplexer::impl {

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  Test, whether the given string represents a valid trigraph sequence
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename StringT>
    constexpr bool is_trigraph(StringT const& trigraph) noexcept
    {
        if (trigraph.size() < 3 || '?' != trigraph[0] || '?' != trigraph[1])
            return false;

        switch (trigraph[2])
        {
        case '\'':
        case '=':
        case '/':
        case '(':
        case ')':
        case '<':
        case '>':
        case '!':
        case '-':
            break;

        default:
            return false;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  convert_trigraph
    //
    //    The function convert_trigraph() converts a single trigraph character
    //    sequence into the corresponding character.
    //
    //    If the given character sequence doesn't form a valid trigraph sequence
    //    no conversion is performed.
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename StringT>
    StringT convert_trigraph(StringT const& trigraph)
    {
        StringT result(trigraph);

        if (is_trigraph(trigraph))
        {
            switch (trigraph[2])
            {
            case '\'':
                result = "^";
                break;
            case '=':
                result = "#";
                break;
            case '/':
                result = "\\";
                break;
            case '(':
                result = "[";
                break;
            case ')':
                result = "]";
                break;
            case '<':
                result = "{";
                break;
            case '>':
                result = "}";
                break;
            case '!':
                result = "|";
                break;
            case '-':
                result = "~";
                break;
            default:
                assert(false);
                break;
            }
        }
        return result;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //
    //  convert_trigraphs
    //
    //    The function convert_trigraph() converts all trigraphs in the given
    //    string into the corresponding characters.
    //
    //    If one of the given character sequences doesn't form a valid trigraph
    //    sequence no conversion is performed.
    //
    ///////////////////////////////////////////////////////////////////////////////
    template <typename StringT>
    StringT convert_trigraphs(StringT const& value)
    {
        StringT result;
        typename StringT::size_type pos = 0;
        typename StringT::size_type pos1 = value.find_first_of("?", 0);
        if (StringT::npos != pos1)
        {
            do
            {
                result += value.substr(pos, pos1 - pos);
                if (StringT trigraph(value.substr(pos1)); is_trigraph(trigraph))
                {
                    result += convert_trigraph(trigraph);
                    pos1 = value.find_first_of("?", pos = pos1 + 3);
                }
                else
                {
                    result += value[pos1];
                    pos1 = value.find_first_of("?", pos = pos1 + 1);
                }
            } while (StringT::npos != pos1);
            result += value.substr(pos);
        }
        else
        {
            result = value;
        }
        return result;
    }
}    // namespace wave::cpplexer::impl
