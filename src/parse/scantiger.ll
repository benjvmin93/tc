                                                            /* -*- C++ -*- */
%option c++
%option nounput
%option debug
%option batch

%{

#include <cerrno>
#include <climits>
#include <regex>
#include <string>

#include <boost/lexical_cast.hpp>

#include <misc/contract.hh>
  // Using misc::escape is very useful to quote non printable characters.
  // For instance
  //
  //    std::cerr << misc::escape('\n') << '\n';
  //
  // reports about `\n' instead of an actual new-line character.
#include <misc/escape.hh>
#include <misc/symbol.hh>
#include <parse/parsetiger.hh>
#include <parse/tiger-parser.hh>
  /* FIXME: Some code was deleted here. */

#include <iostream>

/* Convenient shortcuts. */
#define TOKEN_VAL(Type, Value)                  \
  parser::make_ ## Type(Value, tp.location_)

#define TOKEN(Type)                             \
  parser::make_ ## Type(tp.location_)


// Flex uses `0' for end of file.  0 is not a token_type.
#define yyterminate() return TOKEN(EOF)

# define CHECK_EXTENSION()                              \
  do {                                                  \
    if (!tp.enable_extensions_p_)                       \
      tp.error_ << misc::error::error_type::scan        \
                << tp.location_                         \
                << ": invalid identifier: `"            \
                << misc::escape(yytext) << "'\n";       \
  } while (false)

YY_FLEX_NAMESPACE_BEGIN
%}

%x SC_COMMENT SC_STRING

/* Abbreviations.  */
int             [0-9]+
digit           [0-9]
xnum            \x[0-9a-fA-F]{2}
character       [a-zA-Z]
object-related  "class" | "extends" | "method" | "new"
symbols         "," | ":" | ";" | "(" | ")" | "[" | "]" | "{" | "}" | "." | "+" | "-" | "*" | "/" | "=" | "<>" | "<" | "<=" | ">" | ">=" | "&" | "|" | ":="
white           [ \t]
end-of-line     "\n\r" | "\r\n" | "\r" | "\n"
words           [a-zA-Z]+
identifier      {character} { {character} | {num} | "_" } | "_main"
%%
%{
  // FIXME: Some code was deleted here (Local variables).
std::string grown_string;
int comments = 0;
int lines = 1;

  // Each time yylex is called.
  tp.location_.step();
%}

 /* The rules.  */

{int}         {
                int val = 0;
                val = strtol(yytext, 0, 10);
                if (val > 255)
                  std::cerr << "Integer too long\n";
                return TOKEN_VAL(INT, val);
              }

"array" {
    return TOKEN(ARRAY);
}
"if" {
    return TOKEN(IF);
}
"then" {
    return TOKEN(THEN);
}
"else" {
    return TOKEN(ELSE);
}
"while" {
    return TOKEN(WHILE);
}
"for" {
    return TOKEN(FOR);
}
"to" {
    return TOKEN(TO);
}
"do" {
    return TOKEN(LET);
}
"let" {
    return TOKEN(LET);
}
"in" {
    return TOKEN(IN);
}
"end" {
    return TOKEN(END);
}
"of" {
    return TOKEN(OF);
}
"break" {
    return TOKEN(BREAK);
}
"nil" {
    return TOKEN(NIL);
}
"function" {
    return TOKEN(FUNCTION);
}
"var" {
    return TOKEN(VAR);
}
"type" {
    return TOKEN(TYPE);
}

"import" {
    return TOKEN(IMPORT);
}

"primitive" {
    return TOKEN(PRIMITIVE);
}

 /* Id. TODO */

 /* Begin of a string. */

"\"" {
  grown_string.clear();
  BEGIN SC_STRING;
}

  /* Begin of a comment. */

"/*" {
  BEGIN SC_COMMENT;
  comments++;
}

  /* End comment before start. */

"*/" {
  std::cerr << "End comment before start\n";
}

  /* String state. */

<SC_STRING>
{
  "\"" {
    BEGIN INITIAL;
    return TOKEN_VAL(STRING, grown_string);
  }
  \\[abfnrtv] { grown_string.append(yytext); }
  {xnum} { grown_string.append(1, strtol(yytext + 2, 0, 16)); }
  {int} { grown_string.append(1, strtol(yytext + 3, 0, 10)); }
  "\\" { grown_string.append(yytext); }
  "\\\"" { grown_string.append(yytext); }
  {character} { grown_string.append(yytext); }
  <<EOF>> { std::cerr << "Unexpected end of file inside string\n"; }
}

  /* Comment state. */

<SC_COMMENT>
{
  "/*" {
    comments++;
    BEGIN SC_COMMENT;
  }
  "*/" {
    comments--;
    if (comments == 0) {
      BEGIN INITIAL;
    }
  }
  <<EOF>> {
    std::cerr << "expected closed comment but got EOF\n";
  }
}

. { std::cerr << "error\n"; }

%%

// Do not use %option noyywrap, because then flex generates the same
// definition of yywrap, but outside the namespaces, so it defines it
// for ::yyFlexLexer instead of ::parse::yyFlexLexer.
int yyFlexLexer::yywrap() { return 1; }

void
yyFlexLexer::scan_open_(std::istream& f)
{
  yypush_buffer_state(YY_CURRENT_BUFFER);
  yy_switch_to_buffer(yy_create_buffer(&f, YY_BUF_SIZE));
}

void
yyFlexLexer::scan_close_()
{
  yypop_buffer_state();
}

YY_FLEX_NAMESPACE_END
