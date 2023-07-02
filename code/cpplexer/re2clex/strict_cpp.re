/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Copyright (c) 2001 Daniel C. Nuffer
    Copyright (c) 2001-2011 Hartmut Kaiser.
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is a strict lexer conforming to the Standard as close as possible.
    It does not allow the '$' to be part of identifiers. If you need the '$'
    character in identifiers please include the lexer definition provided
    in the cpp.re file.

    TODO:
        handle errors better.
=============================================================================*/

/*!re2c
re2c:indent:string = "    ";
any                = [\t\v\f\r\n\040-\377];
anyctrl            = [\001-\037];
OctalDigit         = [0-7];
Digit              = [0-9];
HexDigit           = [a-fA-F0-9];
Integer            = (("0" [xX] HexDigit+) | ("0" OctalDigit*) | ([1-9] Digit*));
ExponentStart      = [Ee] [+-];
ExponentPart       = [Ee] [+-]? Digit+;
FractionalConstant = (Digit* "." Digit+) | (Digit+ ".");
FloatingSuffix     = [fF] [lL]? | [lL] [fF]?;
IntegerSuffix      = [uU] [lL]? | [lL] [uU]?;
LongIntegerSuffix  = [uU] ("ll" | "LL") | ("ll" | "LL") [uU]?;
Backslash          = [\\] | "??/";
EscapeSequence     = Backslash ([abfnrtv?'"] | Backslash | "x" HexDigit+ | OctalDigit OctalDigit? OctalDigit?);
HexQuad            = HexDigit HexDigit HexDigit HexDigit;
UniversalChar      = Backslash ("u" HexQuad | "U" HexQuad HexQuad);
Newline            = "\r\n" | "\n" | "\r";
PPSpace            = ([ \t\f\v]|("/*"(any\[*]|Newline|("*"+(any\[*/]|Newline)))*"*"+"/"))*;
Pound              = "#" | "??=" | "%:";
NonDigit           = [a-zA-Z_] | UniversalChar;
*/

/*!re2c
    "/*"            { goto ccomment; }
    "//"            { goto cppcomment; }
    "."? Digit      { goto pp_number; }

    "alignas"       { WAVE_RET(s->act_in_cpp0x_mode ? T_ALIGNAS : T_IDENTIFIER); }
    "alignof"       { WAVE_RET(s->act_in_cpp0x_mode ? T_ALIGNOF : T_IDENTIFIER); }
    "asm"           { WAVE_RET(T_ASM); }
    "auto"          { WAVE_RET(T_AUTO); }
    "bool"          { WAVE_RET(T_BOOL); }
    "break"         { WAVE_RET(T_BREAK); }
    "case"          { WAVE_RET(T_CASE); }
    "catch"         { WAVE_RET(T_CATCH); }
    "char"          { WAVE_RET(T_CHAR); }
    "char8_t"       { WAVE_RET(s->act_in_cpp2a_mode ? T_CHAR8_T : T_IDENTIFIER); }
    "char16_t"      { WAVE_RET(s->act_in_cpp0x_mode ? T_CHAR16_T : T_IDENTIFIER); }
    "char32_t"      { WAVE_RET(s->act_in_cpp0x_mode ? T_CHAR32_T : T_IDENTIFIER); }
    "class"         { WAVE_RET(T_CLASS); }
    "concept"       { WAVE_RET(s->act_in_cpp2a_mode ? T_CONCEPT : T_IDENTIFIER); }
    "const"         { WAVE_RET(T_CONST); }
    "consteval"     { WAVE_RET(s->act_in_cpp2a_mode ? T_CONSTEVAL : T_IDENTIFIER); }
    "constexpr"     { WAVE_RET(s->act_in_cpp0x_mode ? T_CONSTEXPR : T_IDENTIFIER); }
    "constinit"     { WAVE_RET(s->act_in_cpp2a_mode ? T_CONSTINIT : T_IDENTIFIER); }
    "const_cast"    { WAVE_RET(T_CONSTCAST); }
    "continue"      { WAVE_RET(T_CONTINUE); }
    "co_await"      { WAVE_RET(s->act_in_cpp2a_mode ? T_CO_AWAIT : T_IDENTIFIER); }
    "co_return"     { WAVE_RET(s->act_in_cpp2a_mode ? T_CO_RETURN : T_IDENTIFIER); }
    "co_yield"      { WAVE_RET(s->act_in_cpp2a_mode ? T_CO_YIELD : T_IDENTIFIER); }
    "decltype"      { WAVE_RET(s->act_in_cpp0x_mode ? T_DECLTYPE : T_IDENTIFIER); }
    "default"       { WAVE_RET(T_DEFAULT); }
    "delete"        { WAVE_RET(T_DELETE); }
    "do"            { WAVE_RET(T_DO); }
    "double"        { WAVE_RET(T_DOUBLE); }
    "dynamic_cast"  { WAVE_RET(T_DYNAMICCAST); }
    "else"          { WAVE_RET(T_ELSE); }
    "enum"          { WAVE_RET(T_ENUM); }
    "explicit"      { WAVE_RET(T_EXPLICIT); }
    "export"        { WAVE_RET(T_EXPORT); }
    "extern"        { WAVE_RET(T_EXTERN); }
    "false"         { WAVE_RET(T_FALSE); }
    "float"         { WAVE_RET(T_FLOAT); }
    "for"           { WAVE_RET(T_FOR); }
    "friend"        { WAVE_RET(T_FRIEND); }
    "goto"          { WAVE_RET(T_GOTO); }
    "if"            { WAVE_RET(T_IF); }
    "import"        { WAVE_RET(s->enable_import_keyword ? T_IMPORT : T_IDENTIFIER); }
    "inline"        { WAVE_RET(T_INLINE); }
    "int"           { WAVE_RET(T_INT); }
    "long"          { WAVE_RET(T_LONG); }
    "mutable"       { WAVE_RET(T_MUTABLE); }
    "namespace"     { WAVE_RET(T_NAMESPACE); }
    "new"           { WAVE_RET(T_NEW); }
    "noexcept"      { WAVE_RET(s->act_in_cpp0x_mode ? T_NOEXCEPT : T_IDENTIFIER); }
    "nullptr"       { WAVE_RET(s->act_in_cpp0x_mode ? T_NULLPTR : T_IDENTIFIER); }
    "operator"      { WAVE_RET(T_OPERATOR); }
    "private"       { WAVE_RET(T_PRIVATE); }
    "protected"     { WAVE_RET(T_PROTECTED); }
    "public"        { WAVE_RET(T_PUBLIC); }
    "register"      { WAVE_RET(T_REGISTER); }
    "reinterpret_cast" { WAVE_RET(T_REINTERPRETCAST); }
    "requires"      { WAVE_RET(s->act_in_cpp2a_mode ? T_REQUIRES : T_IDENTIFIER); }
    "return"        { WAVE_RET(T_RETURN); }
    "short"         { WAVE_RET(T_SHORT); }
    "signed"        { WAVE_RET(T_SIGNED); }
    "sizeof"        { WAVE_RET(T_SIZEOF); }
    "static"        { WAVE_RET(T_STATIC); }
    "static_cast"   { WAVE_RET(T_STATICCAST); }
    "static_assert" { WAVE_RET(s->act_in_cpp0x_mode ? T_STATICASSERT : T_IDENTIFIER); }
    "struct"        { WAVE_RET(T_STRUCT); }
    "switch"        { WAVE_RET(T_SWITCH); }
    "template"      { WAVE_RET(T_TEMPLATE); }
    "this"          { WAVE_RET(T_THIS); }
    "thread_local"  { WAVE_RET(s->act_in_cpp0x_mode ? T_THREADLOCAL : T_IDENTIFIER); }
    "throw"         { WAVE_RET(T_THROW); }
    "true"          { WAVE_RET(T_TRUE); }
    "try"           { WAVE_RET(T_TRY); }
    "typedef"       { WAVE_RET(T_TYPEDEF); }
    "typeid"        { WAVE_RET(T_TYPEID); }
    "typename"      { WAVE_RET(T_TYPENAME); }
    "union"         { WAVE_RET(T_UNION); }
    "unsigned"      { WAVE_RET(T_UNSIGNED); }
    "using"         { WAVE_RET(T_USING); }
    "virtual"       { WAVE_RET(T_VIRTUAL); }
    "void"          { WAVE_RET(T_VOID); }
    "volatile"      { WAVE_RET(T_VOLATILE); }
    "wchar_t"       { WAVE_RET(T_WCHART); }
    "while"         { WAVE_RET(T_WHILE); }

    "__int8"        { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_INT8 : T_IDENTIFIER); }
    "__int16"       { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_INT16 : T_IDENTIFIER); }
    "__int32"       { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_INT32 : T_IDENTIFIER); }
    "__int64"       { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_INT64 : T_IDENTIFIER); }
    "_"? "_based"   { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_BASED : T_IDENTIFIER); }
    "_"? "_declspec" { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_DECLSPEC : T_IDENTIFIER); }
    "_"? "_cdecl"   { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_CDECL : T_IDENTIFIER); }
    "_"? "_fastcall" { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_FASTCALL : T_IDENTIFIER); }
    "_"? "_stdcall" { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_STDCALL : T_IDENTIFIER); }
    "__try"         { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_TRY : T_IDENTIFIER); }
    "__except"      { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_EXCEPT : T_IDENTIFIER); }
    "__finally"     { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_FINALLY : T_IDENTIFIER); }
    "__leave"       { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_LEAVE : T_IDENTIFIER); }
    "_"? "_inline"  { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_INLINE : T_IDENTIFIER); }
    "_"? "_asm"     { WAVE_RET(s->enable_ms_extensions ? T_MSEXT_ASM : T_IDENTIFIER); }

    "{"             { WAVE_RET(T_LEFTBRACE); }
    "??<"           { WAVE_RET(T_LEFTBRACE_TRIGRAPH); }
    "<%"            { WAVE_RET(T_LEFTBRACE_ALT); }
    "}"             { WAVE_RET(T_RIGHTBRACE); }
    "??>"           { WAVE_RET(T_RIGHTBRACE_TRIGRAPH); }
    "%>"            { WAVE_RET(T_RIGHTBRACE_ALT); }
    "["             { WAVE_RET(T_LEFTBRACKET); }
    "??("           { WAVE_RET(T_LEFTBRACKET_TRIGRAPH); }
    "<:"            { WAVE_RET(T_LEFTBRACKET_ALT); }
    "]"             { WAVE_RET(T_RIGHTBRACKET); }
    "??)"           { WAVE_RET(T_RIGHTBRACKET_TRIGRAPH); }
    ":>"            { WAVE_RET(T_RIGHTBRACKET_ALT); }
    "#"             { WAVE_RET(T_POUND); }
    "%:"            { WAVE_RET(T_POUND_ALT); }
    "??="           { WAVE_RET(T_POUND_TRIGRAPH); }
    "##"            { WAVE_RET(T_POUND_POUND); }
    "#??="          { WAVE_RET(T_POUND_POUND_TRIGRAPH); }
    "??=#"          { WAVE_RET(T_POUND_POUND_TRIGRAPH); }
    "??=??="        { WAVE_RET(T_POUND_POUND_TRIGRAPH); }
    "%:%:"          { WAVE_RET(T_POUND_POUND_ALT); }
    "("             { WAVE_RET(T_LEFTPAREN); }
    ")"             { WAVE_RET(T_RIGHTPAREN); }
    ";"             { WAVE_RET(T_SEMICOLON); }
    ":"             { WAVE_RET(T_COLON); }
    "..."           { WAVE_RET(T_ELLIPSIS); }
    "?"             { WAVE_RET(T_QUESTION_MARK); }
    "::"
        {
            if (s->act_in_c99_mode) {
                --YYCURSOR;
                WAVE_RET(T_COLON);
            }
            else {
                WAVE_RET(T_COLON_COLON);
            }
        }
    "."             { WAVE_RET(T_DOT); }
    ".*"
        {
            if (s->act_in_c99_mode) {
                --YYCURSOR;
                WAVE_RET(T_DOT);
            }
            else {
                WAVE_RET(T_DOTSTAR);
            }
        }
    "+"             { WAVE_RET(T_PLUS); }
    "-"             { WAVE_RET(T_MINUS); }
    "*"             { WAVE_RET(T_STAR); }
    "/"             { WAVE_RET(T_DIVIDE); }
    "%"             { WAVE_RET(T_PERCENT); }
    "^"             { WAVE_RET(T_XOR); }
    "??'"           { WAVE_RET(T_XOR_TRIGRAPH); }
    "xor"           { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_XOR_ALT); }
    "&"             { WAVE_RET(T_AND); }
    "bitand"        { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_AND_ALT); }
    "|"             { WAVE_RET(T_OR); }
    "bitor"         { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_OR_ALT); }
    "??!"           { WAVE_RET(T_OR_TRIGRAPH); }
    "~"             { WAVE_RET(T_COMPL); }
    "??-"           { WAVE_RET(T_COMPL_TRIGRAPH); }
    "compl"         { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_COMPL_ALT); }
    "!"             { WAVE_RET(T_NOT); }
    "not"           { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_NOT_ALT); }
    "="             { WAVE_RET(T_ASSIGN); }
    "<"             { WAVE_RET(T_LESS); }
    ">"             { WAVE_RET(T_GREATER); }
    "+="            { WAVE_RET(T_PLUSASSIGN); }
    "-="            { WAVE_RET(T_MINUSASSIGN); }
    "*="            { WAVE_RET(T_STARASSIGN); }
    "/="            { WAVE_RET(T_DIVIDEASSIGN); }
    "%="            { WAVE_RET(T_PERCENTASSIGN); }
    "^="            { WAVE_RET(T_XORASSIGN); }
    "xor_eq"        { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_XORASSIGN_ALT); }
    "??'="          { WAVE_RET(T_XORASSIGN_TRIGRAPH); }
    "&="            { WAVE_RET(T_ANDASSIGN); }
    "and_eq"        { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_ANDASSIGN_ALT); }
    "|="            { WAVE_RET(T_ORASSIGN); }
    "or_eq"         { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_ORASSIGN_ALT); }
    "??!="          { WAVE_RET(T_ORASSIGN_TRIGRAPH); }
    "<<"            { WAVE_RET(T_SHIFTLEFT); }
    ">>"            { WAVE_RET(T_SHIFTRIGHT); }
    ">>="           { WAVE_RET(T_SHIFTRIGHTASSIGN); }
    "<<="           { WAVE_RET(T_SHIFTLEFTASSIGN); }
    "=="            { WAVE_RET(T_EQUAL); }
    "!="            { WAVE_RET(T_NOTEQUAL); }
    "not_eq"        { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_NOTEQUAL_ALT); }
    "<=>"
        {
            if (s->act_in_cpp2a_mode) {
                WAVE_RET(T_SPACESHIP);
            }
            else {
                --YYCURSOR;
                WAVE_RET(T_LESSEQUAL);
            }
        }
    "<="            { WAVE_RET(T_LESSEQUAL); }
    ">="            { WAVE_RET(T_GREATEREQUAL); }
    "&&"            { WAVE_RET(T_ANDAND); }
    "and"           { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_ANDAND_ALT); }
    "||"            { WAVE_RET(T_OROR); }
    "??!|"          { WAVE_RET(T_OROR_TRIGRAPH); }
    "|??!"          { WAVE_RET(T_OROR_TRIGRAPH); }
    "or"            { WAVE_RET(s->act_in_c99_mode ? T_IDENTIFIER : T_OROR_ALT); }
    "??!??!"        { WAVE_RET(T_OROR_TRIGRAPH); }
    "++"            { WAVE_RET(T_PLUSPLUS); }
    "--"            { WAVE_RET(T_MINUSMINUS); }
    ","             { WAVE_RET(T_COMMA); }
    "->*"
        {
            if (s->act_in_c99_mode) {
                --YYCURSOR;
                WAVE_RET(T_ARROW);
            }
            else {
                WAVE_RET(T_ARROWSTAR);
            }
        }
    "->"            { WAVE_RET(T_ARROW); }
    "??/"           { WAVE_RET(T_ANY_TRIGRAPH); }

    "L"? (['] (EscapeSequence | UniversalChar | any\[\n\r\\'])+ ['])
        { WAVE_RET(T_CHARLIT); }

    "L"? (["] (EscapeSequence | UniversalChar | any\[\n\r\\"])* ["])
        { WAVE_RET(T_STRINGLIT); }

    "L"? "R" ["]
        {
            if (s->act_in_cpp0x_mode)
            {
                rawstringdelim = "";
                goto extrawstringlit;
            }
            --YYCURSOR;
            WAVE_RET(T_IDENTIFIER);
        }

    [uU] [']
        {
            if (s->act_in_cpp0x_mode)
                goto extcharlit;
            --YYCURSOR;
            WAVE_RET(T_IDENTIFIER);
        }

    ([uU] | "u8") ["]
        {
            if (s->act_in_cpp0x_mode)
                goto extstringlit;
            --YYCURSOR;
            WAVE_RET(T_IDENTIFIER);
        }

    ([uU] | "u8") "R" ["]
        {
            if (s->act_in_cpp0x_mode)
            {
                rawstringdelim = "";
                goto extrawstringlit;
            }
            --YYCURSOR;
            WAVE_RET(T_IDENTIFIER);
        }

    ([a-zA-Z_] | UniversalChar) ([a-zA-Z_0-9] | UniversalChar)*
        { WAVE_RET(T_IDENTIFIER); }

    Pound PPSpace ( "include" | "include_next") PPSpace "<" (any\[\n\r>])+ ">"
        { WAVE_RET(T_PP_HHEADER); }

    Pound PPSpace ( "include" | "include_next") PPSpace "\"" (any\[\n\r"])+ "\""
        { WAVE_RET(T_PP_QHEADER); }

    Pound PPSpace ( "include" | "include_next") PPSpace
        { WAVE_RET(T_PP_INCLUDE); }

    Pound PPSpace "if"        { WAVE_RET(T_PP_IF); }
    Pound PPSpace "ifdef"     { WAVE_RET(T_PP_IFDEF); }
    Pound PPSpace "ifndef"    { WAVE_RET(T_PP_IFNDEF); }
    Pound PPSpace "else"      { WAVE_RET(T_PP_ELSE); }
    Pound PPSpace "elif"      { WAVE_RET(T_PP_ELIF); }
    Pound PPSpace "endif"     { WAVE_RET(T_PP_ENDIF); }
    Pound PPSpace "define"    { WAVE_RET(T_PP_DEFINE); }
    Pound PPSpace "undef"     { WAVE_RET(T_PP_UNDEF); }
    Pound PPSpace "line"      { WAVE_RET(T_PP_LINE); }
    Pound PPSpace "error"     { WAVE_RET(T_PP_ERROR); }
    Pound PPSpace "pragma"    { WAVE_RET(T_PP_PRAGMA); }

    Pound PPSpace "warning"   { WAVE_RET(T_PP_WARNING); }

    Pound PPSpace "region"    { WAVE_RET(T_MSEXT_PP_REGION); }
    Pound PPSpace "endregion" { WAVE_RET(T_MSEXT_PP_ENDREGION); }

    [ \t\v\f]+
        { WAVE_RET(T_SPACE); }

    Newline
    {
        s->line++;
        cursor.column = 1;
        WAVE_RET(T_NEWLINE);
    }

    "\000"
    {
        if (s->eof && cursor != s->eof)
        {
            WAVE_UPDATE_CURSOR();     // adjust the input cursor
            (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                "invalid character '\\000' in input stream");
        }
        WAVE_RET(T_EOF);
    }

    any        { WAVE_RET(TOKEN_FROM_ID(*s->tok, UnknownTokenType)); }

    anyctrl
    {
        // flag the error
        WAVE_UPDATE_CURSOR();     // adjust the input cursor
        (*s->error_proc)(s, lexing_exception::generic_lexing_error,
            "invalid character '\\%03o' in input stream", *--YYCURSOR);
    }
*/

ccomment:
/*!re2c
    "*/"            { WAVE_RET(T_CCOMMENT); }

    Newline
    {
        /*if(cursor == s->eof) WAVE_RET(T_EOF);*/
        /*s->tok = cursor; */
        s->line += count_backslash_newlines(s, cursor) +1;
        cursor.column = 1;
        goto ccomment;
    }

    any            { goto ccomment; }

    "\000"
    {
        if(cursor == s->eof)
        {
            WAVE_UPDATE_CURSOR();   // adjust the input cursor
            (*s->error_proc)(s, lexing_exception::generic_lexing_warning,
                "Unterminated 'C' style comment");
        }
        else
        {
            --YYCURSOR;                   // next call returns T_EOF
            WAVE_UPDATE_CURSOR();   // adjust the input cursor
            (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                "invalid character: '\\000' in input stream");
        }
    }

    anyctrl
    {
        // flag the error
        WAVE_UPDATE_CURSOR();   // adjust the input cursor
        (*s->error_proc)(s, lexing_exception::generic_lexing_error,
            "invalid character '\\%03o' in input stream", *--YYCURSOR);
    }
*/

cppcomment:
/*!re2c
    Newline
    {
        /*if(cursor == s->eof) WAVE_RET(T_EOF); */
        /*s->tok = cursor; */
        s->line++;
        cursor.column = 1;
        WAVE_RET(T_CPPCOMMENT);
    }

    any            { goto cppcomment; }

    "\000"
    {
        if (s->eof && cursor != s->eof)
        {
            --YYCURSOR;                     // next call returns T_EOF
            WAVE_UPDATE_CURSOR();     // adjust the input cursor
            (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                "invalid character '\\000' in input stream");
        }

        --YYCURSOR;                         // next call returns T_EOF
        if (!s->single_line_only)
        {
            WAVE_UPDATE_CURSOR();     // adjust the input cursor
            (*s->error_proc)(s, lexing_exception::generic_lexing_warning,
                "Unterminated 'C++' style comment");
        }
        WAVE_RET(T_CPPCOMMENT);
    }

    anyctrl
    {
        // flag the error
        WAVE_UPDATE_CURSOR();     // adjust the input cursor
        (*s->error_proc)(s, lexing_exception::generic_lexing_error,
            "invalid character '\\%03o' in input stream", *--YYCURSOR);
    }
*/

/* this subscanner is called whenever a pp_number has been started */
pp_number:
{
    cursor = uchar_wrapper(s->tok = s->cur, s->column = s->curr_column);
    marker = uchar_wrapper(s->ptr);
    limit = uchar_wrapper(s->lim);

    if (s->detect_pp_numbers) {
    /*!re2c
        "."? Digit (Digit | NonDigit | ExponentStart | ".")*
            { WAVE_RET(T_PP_NUMBER); }

        * { ASSERT(false); }
    */
    }
    else {
    /*!re2c
        ((FractionalConstant ExponentPart?) | (Digit+ ExponentPart)) FloatingSuffix?
            { WAVE_RET(T_FLOATLIT); }

        Integer { goto integer_suffix; }

        * { ASSERT(false); }
    */
    }
}

/* this subscanner is called, whenever an Integer was recognized */
integer_suffix:
{
    if (s->enable_ms_extensions) {
    /*!re2c
        LongIntegerSuffix | "i64"
            { WAVE_RET(T_LONGINTLIT); }

        IntegerSuffix?
            { WAVE_RET(T_INTLIT); }
    */
    }
    else {
    /*!re2c
        LongIntegerSuffix
            { WAVE_RET(T_LONGINTLIT); }

        IntegerSuffix?
            { WAVE_RET(T_INTLIT); }
    */
    }

    // re2c will complain about -Wmatch-empty-string above
    // it's OK because we've already matched an integer
    // and will return T_INTLIT
}

/* this subscanner is invoked for C++0x extended character literals */
extcharlit:
{
    /*!re2c
        * {
            (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                "Invalid character in raw string delimiter ('%c')", yych);
        }

        ((EscapeSequence | UniversalChar | any\[\n\r\\']) ['])
            { WAVE_RET(T_CHARLIT); }

        any
            { WAVE_RET(TOKEN_FROM_ID(*s->tok, UnknownTokenType)); }
    */
}

/* this subscanner is invoked for C++0x extended character string literals */
extstringlit:
{
    /*!re2c
        * {
            (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                "Invalid character in raw string delimiter ('%c')", yych);
        }

        ((EscapeSequence | UniversalChar | any\[\n\r\\"])* ["])
            { WAVE_RET(T_STRINGLIT); }

        any
            { WAVE_RET(TOKEN_FROM_ID(*s->tok, UnknownTokenType)); }
    */
}

extrawstringlit:
{
    // we have consumed the double quote but not the lparen
    // at this point we may see a delimiter

    /*!re2c
        * {
            (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                "Invalid character in raw string delimiter ('%c')", yych);
        }

        // delimiters are any character but parentheses, backslash, and whitespace
        any\[()\\\t\v\f\r\n]
        {
            rawstringdelim += yych;
            if (rawstringdelim.size() > 16)
            {
                (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                    "Raw string delimiter of excessive length (\"%s\") in input stream",
                    rawstringdelim.c_str());
            }
            goto extrawstringlit;
        }

        "("
        {
            rawstringdelim = ")" + rawstringdelim;
            goto extrawstringbody;
        }

    */
}

extrawstringbody:
{
    /*!re2c

        * {
            (*s->error_proc)(s, lexing_exception::generic_lexing_error,
                "Invalid character in raw string body ('%c')", yych);
        }

        Newline
        {
            s->line += count_backslash_newlines(s, cursor) +1;
            cursor.column = 1;
            goto extrawstringbody;
        }

        (EscapeSequence | UniversalChar | any\["])
        {
            goto extrawstringbody;
        }

        ["]
        {
            // check to see if we have completed a delimiter
            if (string_type((char *)(YYCURSOR - rawstringdelim.size() - 1),
                             (char *)(YYCURSOR - 1)) == rawstringdelim)
            {
                 WAVE_RET(T_RAWSTRINGLIT);
            } else {
                goto extrawstringbody;
            }
        }
    */
}
