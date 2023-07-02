/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Global application configuration

    http://www.boost.org/

    Copyright (c) 2001-2012 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#pragma once

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether to support variadics and placemarkers
//
//  To implement support variadics and placemarkers define the following to
//  something not equal to zero.
//
#if !defined(WAVE_SUPPORT_VARIADICS_PLACEMARKERS)
#define WAVE_SUPPORT_VARIADICS_PLACEMARKERS 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide whether to support the C++20 __VA_OPT__ variadics feature
//
//
#if !defined(WAVE_SUPPORT_VA_OPT) && WAVE_SUPPORT_VARIADICS_PLACEMARKERS
#define WAVE_SUPPORT_VA_OPT 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether to implement a #warning directive as an extension to the
//  C++ Standard (same as #error, but emits a warning, not an error)
//
//  To disable the implementation of #warning directives, define the following
//  constant as zero before including this file.
//
#if !defined(WAVE_SUPPORT_WARNING_DIRECTIVE)
#define WAVE_SUPPORT_WARNING_DIRECTIVE 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether to implement #pragma message("")
//
//  To disable the implementation of #pragma message(""), define the following
//  constant as zero before including this file.
//
#if !defined(WAVE_SUPPORT_PRAGMA_MESSAGE)
#define WAVE_SUPPORT_PRAGMA_MESSAGE 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether to implement #include_next
//  Please note, that this is an extension to the C++ Standard.
//
//  To disable the implementation of #include_next, define the following
//  constant as zero before including this file.
//
#if !defined(WAVE_SUPPORT_INCLUDE_NEXT)
#define WAVE_SUPPORT_INCLUDE_NEXT 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether to support C++11
//
//  To implement C++11 keywords and preprocessor semantics define the following
//  to something not equal to zero.
//
#if !defined(WAVE_SUPPORT_CPP0X)
#define WAVE_SUPPORT_CPP0X 1
#undef WAVE_SUPPORT_VARIADICS_PLACEMARKERS
#define WAVE_SUPPORT_VARIADICS_PLACEMARKERS 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide whether to support C++17
//
#if !defined(WAVE_SUPPORT_CPP1Z)
#define WAVE_SUPPORT_CPP1Z 1
#undef WAVE_SUPPORT_CPP0X
#define WAVE_SUPPORT_CPP0X 1
#undef WAVE_SUPPORT_VARIADICS_PLACEMARKERS
#define WAVE_SUPPORT_VARIADICS_PLACEMARKERS 1
#if !defined(WAVE_SUPPORT_HAS_INCLUDE)
#define WAVE_SUPPORT_HAS_INCLUDE 1
#endif
#elif WAVE_SUPPORT_CPP1Z == 0
#undef WAVE_SUPPORT_HAS_INCLUDE
#define WAVE_SUPPORT_HAS_INCLUDE 0
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide whether to support C++20
//
#if !defined(WAVE_SUPPORT_CPP2A)
#define WAVE_SUPPORT_CPP2A 1
#undef WAVE_SUPPORT_CPP0X
#define WAVE_SUPPORT_CPP0X 1
#undef WAVE_SUPPORT_VARIADICS_PLACEMARKERS
#define WAVE_SUPPORT_VARIADICS_PLACEMARKERS 1
#undef WAVE_SUPPORT_CPP1Z
#define WAVE_SUPPORT_CPP1Z 1
#if !defined(WAVE_SUPPORT_HAS_INCLUDE)
#define WAVE_SUPPORT_HAS_INCLUDE 1
#endif
#if !defined(WAVE_SUPPORT_VA_OPT)
#define WAVE_SUPPORT_VA_OPT 1
#endif
#elif WAVE_SUPPORT_CPP2A == 0
#undef WAVE_SUPPORT_VA_OPT
#define WAVE_SUPPORT_VA_OPT 0
#endif

///////////////////////////////////////////////////////////////////////////////
//  Allow the message body of the #error and #warning directives to be
//  preprocessed before the diagnostic is issued.
//
//  To disable preprocessing of the body of #error and #warning directives,
//  define the following constant as zero before including this file.
//
#if !defined(WAVE_PREPROCESS_ERROR_MESSAGE_BODY)
#define WAVE_PREPROCESS_ERROR_MESSAGE_BODY 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Allow the #pragma directives to be returned to the caller (optionally after
//  preprocessing the body)
//
//  To disable the library to emit unknown #pragma directives, define the
//  following constant as zero before including this file.
//
#if !defined(WAVE_EMIT_PRAGMA_DIRECTIVES)
#define WAVE_EMIT_PRAGMA_DIRECTIVES 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Allow the body of a #pragma directive to be preprocessed before the
//  directive is returned to the caller.
//
//  To disable the preprocessing of the body of #pragma directives, define the
//  following constant as zero before including this file.
//
#if !defined(WAVE_PREPROCESS_PRAGMA_BODY)
#define WAVE_PREPROCESS_PRAGMA_BODY 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Allow to define macros with the command line syntax (-DMACRO(x)=definition)
//
//  To disable the possibility to define macros based on the command line
//  syntax, define the following constant as zero before including this file.
//
#if !defined(WAVE_ENABLE_COMMANDLINE_MACROS)
#define WAVE_ENABLE_COMMANDLINE_MACROS 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Define the pragma keyword to be used by the library to recognize its own
//  pragma's. If nothing else is defined, then 'wave' will be used as the
//  default keyword, i.e. the library recognizes all
//
//      #pragma wave option [(argument)]
//
//  and dispatches the handling to the interpret_pragma() preprocessing hook
//  function (see file: preprocessing_hooks.hpp). The arguments part of the
//  pragma is optional.
//  The WAVE_PRAGMA_KEYWORD constant needs to be defined to
//  resolve as a string literal value.
#if !defined(WAVE_PRAGMA_KEYWORD)
#define WAVE_PRAGMA_KEYWORD "wave"
#endif

///////////////////////////////////////////////////////////////////////////////
//  Define the string type to be used to store the token values and the file
//  names inside a file_position template class
//
#define WAVE_STRINGTYPE std::string

///////////////////////////////////////////////////////////////////////////////
//  The following definition forces the Spirit tree code to use list's instead
//  of vectors, which may be more efficient on some platforms
// #define SPIRIT_USE_LIST_FOR_TREES

///////////////////////////////////////////////////////////////////////////////
//  The following definition forces the Spirit tree code to use boost pool
//  allocators instead of the std::allocator for the vector/list's.
// #define SPIRIT_USE_ALLOCATOR_FOR_TREES

///////////////////////////////////////////////////////////////////////////////
//  Uncomment the following, if you need debug output, the
//  SPIRIT_DEBUG_FLAGS_CPP constants below help to fine control the
//  amount of the generated debug output.
//#define SPIRIT_DEBUG

///////////////////////////////////////////////////////////////////////////////
//
//  For all recognized preprocessor statements the output parse trees
//  formatted as xml are printed. The formatted parse trees are streamed to the
//  std::ostream defined by the WAVE_DUMP_PARSE_TREE_OUT constant.
//
//  To enable the output of these parse trees, define the following constant
//  as zero something not equal to zero before including this file.
//
#if !defined(WAVE_DUMP_PARSE_TREE)
#define WAVE_DUMP_PARSE_TREE 0
#endif
#if WAVE_DUMP_PARSE_TREE != 0 && !defined(WAVE_DUMP_PARSE_TREE_OUT)
#define WAVE_DUMP_PARSE_TREE_OUT std::cerr
#endif

///////////////////////////////////////////////////////////////////////////////
//
//  For all #if and #elif directives the preprocessed expressions are printed.
//  These expressions are streamed to the std::ostream defined by the
//  WAVE_DUMP_CONDITIONAL_EXPRESSIONS_OUT constant.
//
//  To enable the output of the preprocessed expressions, define the following
//  constant as something not equal to zero before including this file.
//
#if !defined(WAVE_DUMP_CONDITIONAL_EXPRESSIONS)
#define WAVE_DUMP_CONDITIONAL_EXPRESSIONS 0
#endif
#if WAVE_DUMP_CONDITIONAL_EXPRESSIONS != 0 &&                                  \
    !defined(WAVE_DUMP_CONDITIONAL_EXPRESSIONS_OUT)
#define WAVE_DUMP_CONDITIONAL_EXPRESSIONS_OUT std::cerr
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether to use the separate compilation model for the instantiation
//  of the C++ lexer objects.
//
//  If this is defined, you should explicitly instantiate the C++ lexer
//  template with the correct parameters in a separate compilation unit of
//  your program (see the file instantiate_re2c_lexer.cpp).
//
//  To use the lexer inclusion model, define the following constant as
//  something not equal to zero before including this file.
//
#if !defined(WAVE_SEPARATE_LEXER_INSTANTIATION)
#define WAVE_SEPARATE_LEXER_INSTANTIATION 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide whether to use a strict C++ lexer.
//
//  If this is defined to something != 0, then the C++ lexers recognize the
//  strict C99/C++ basic source character set. If it is not defined or defined
//  to zero, the C++ lexers recognize the '$' character as part of identifiers.
//
//  The default is to recognize the '$' character as part of identifiers.
//
#if !defined(WAVE_USE_STRICT_LEXER)
#define WAVE_USE_STRICT_LEXER 1
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether the import keyword is recognized as such
//
//  If this is defined to something not equal to zero the Wave will recognize
//  import as a keyword (T_IMPORT token id)
//
#if !defined(WAVE_SUPPORT_IMPORT_KEYWORD)
#define WAVE_SUPPORT_IMPORT_KEYWORD 0
#endif

///////////////////////////////////////////////////////////////////////////////
//  Decide, whether to support long long integers in the preprocessor.
//
//  The C++ standard requires the preprocessor to use one of the following
//  types for integer literals: long or unsigned long depending on an optional
//  suffix ('u', 'l', 'ul', or 'lu')
//
//  Sometimes it's required to preprocess integer literals bigger than that
//  (i.e. long long or unsigned long long). In this case you need to define the
//  WAVE_SUPPORT_LONGLONG_INTEGER_LITERALS to something not equal to zero.
//
//  This pp constant is effective only, if your target platform supports
//  long long integers (HAS_LONG_LONG is defined).
//
//  Please note, that this setting doesn't relate to the Wave support option
//  support_option_long_long, which enables the recognition of 'll' suffixes
//  only.
//
//  Defining this pp constant enables the recognition of long long integers
//  even if these do not have the 'll' suffix.
//
#if !defined(WAVE_SUPPORT_LONGLONG_INTEGER_LITERALS)
#define WAVE_SUPPORT_LONGLONG_INTEGER_LITERALS 0
#endif

namespace wave {
    using int_literal_type = long long;
    using uint_literal_type = unsigned long long;
}    // namespace wave

///////////////////////////////////////////////////////////////////////////////
//  On some platforms Wave will not be able to properly detect whether wchar_t
//  is representing a signed or unsigned integral data type. Use the
//  configuration constants below to force wchar_t being signed or unsigned, as
//  appropriate.
//
//  The default is to use std::numeric_limits<wchar_t>::is_signed.

#define WAVE_WCHAR_T_AUTOSELECT 1
#define WAVE_WCHAR_T_FORCE_SIGNED 2
#define WAVE_WCHAR_T_FORCE_UNSIGNED 3

#if !defined(WAVE_WCHAR_T_SIGNEDNESS)
#define WAVE_WCHAR_T_SIGNEDNESS WAVE_WCHAR_T_AUTOSELECT
#endif

#ifndef WAVE_DECL
#define WAVE_DECL
#endif
