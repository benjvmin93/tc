                                                                // -*- C++ -*-
%require "3.0"
%language "C++"
// Set the namespace name to `parse', instead of `yy'.
%define api.prefix {parse}
%define api.value.type variant
%define api.token.constructor

// We use a GLR parser because it is able to handle Shift-Reduce and
// Reduce-Reduce conflicts by forking and testing each way at runtime. GLR
// permits, by allowing few conflicts, more readable and maintainable grammars.
%glr-parser
%skeleton "glr2.cc"

// In TC, we expect the GLR to resolve one Shift-Reduce and zero Reduce-Reduce
// conflict at runtime. Use %expect and %expect-rr to tell Bison about it.
%expect 1
%expect-rr 0
  // FIXME: Some code was deleted here (Other directives).

%define parse.error verbose
%defines
%debug
// Prefix all the tokens with TOK_ to avoid colisions.
%define api.token.prefix {TOK_}

/* We use pointers to store the filename in the locations.  This saves
   space (pointers), time (no deep copy), but leaves the problem of
   deallocation.  This would be a perfect job for a misc::symbol
   object (passed by reference), however Bison locations require the
   filename to be passed as a pointer, thus forcing us to handle the
   allocation and deallocation of this object.

   Nevertheless, all is not lost: we can still use a misc::symbol
   object to allocate a flyweight (constant) string in the pool of
   symbols, extract it from the misc::symbol object, and use it to
   initialize the location.  The allocated data will be freed at the
   end of the program (see the documentation of misc::symbol and
   misc::unique).  */
%define api.filename.type {const std::string}
%locations

// The parsing context.
%param { ::parse::TigerParser& tp }

/*---------------------.
| Support for tokens.  |
`---------------------*/
%code requires
{
#include <string>
#include <misc/algorithm.hh>
#include <misc/separator.hh>
#include <misc/symbol.hh>
#include <parse/fwd.hh>

  // Pre-declare parse::parse to allow a ``reentrant'' parsing within
  // the parser.
  namespace parse
  {
    ast_type parse(Tweast& input);
  }
}

%code provides
{
  // Announce to Flex the prototype we want for lexing (member) function.
  # define YY_DECL_(Prefix)                               \
    ::parse::parser::symbol_type                          \
    (Prefix parselex)(::parse::TigerParser& tp)
  # define YY_DECL YY_DECL_(yyFlexLexer::)
}

%printer { yyo << $$; } <int> <std::string> <misc::symbol>;

%token <std::string>    STRING "string"
%token <misc::symbol>   ID     "identifier"
%token <int>            INT    "integer"



/*-----------------------------------------.
| Code output in the implementation file.  |
`-----------------------------------------*/

%code
{
# include <parse/scantiger.hh>
# include <parse/tweast.hh>
# include <misc/separator.hh>
# include <misc/symbol.hh>

  namespace
  {

    /// Get the metavar from the specified map.
    template <typename T>
    T*
    metavar(parse::TigerParser& tp, unsigned key)
    {
      parse::Tweast* input = tp.input_;
      return input->template take<T>(key);
    }

  }

  /// Use our local scanner object.
  inline
  ::parse::parser::symbol_type
  parselex(parse::TigerParser& tp)
  {
    return tp.scanner_->parselex(tp);
  }
}

// Definition of the tokens, and their pretty-printing.
%token AND          "&"
       ARRAY        "array"
       ASSIGN       ":="
       BREAK        "break"
       CAST         "_cast"
       CLASS        "class"
       COLON        ":"
       COMMA        ","
       DIVIDE       "/"
       DO           "do"
       DOT          "."
       ELSE         "else"
       END          "end"
       EQ           "="
       EXTENDS      "extends"
       FOR          "for"
       FUNCTION     "function"
       GE           ">="
       GT           ">"
       IF           "if"
       IMPORT       "import"
       IN           "in"
       LBRACE       "{"
       LBRACK       "["
       LE           "<="
       LET          "let"
       LPAREN       "("
       LT           "<"
       MINUS        "-"
       METHOD       "method"
       NE           "<>"
       NEW          "new"
       NIL          "nil"
       OF           "of"
       OR           "|"
       PLUS         "+"
       PRIMITIVE    "primitive"
       RBRACE       "}"
       RBRACK       "]"
       RPAREN       ")"
       SEMI         ";"
       THEN         "then"
       TIMES        "*"
       TO           "to"
       TYPE         "type"
       VAR          "var"
       WHILE        "while"
       EOF 0        "end of file"


  // FIXME: Some code was deleted here (Priorities/associativities).

// Solving conflicts on:
// let type foo = bar
//     type baz = bat
// which can be understood as a list of two TypeChunk containing
// a unique TypeDec each, or a single TypeChunk containing two TypeDec.
// We want the latter.
//%right shift
//%left reduce
%precedence CHUNKS
%precedence TYPE
%nonassoc ASSIGN
%right DOT ID
%right WHILE DO
%right THEN ELSE
%right OF
%right CLASS
%right PRIMITIVE FUNCTION
%left TIMES DIVIDE
%left PLUS MINUS
%nonassoc GE LE EQ GT LT NE
%left AND
%left OR

%precedence UMINUS
  // FIXME: Some code was deleted here (Other declarations).

%start program

%%
program:
  /* Parsing a source program.  */
  exp
   
| /* Parsing an imported file.  */
  chunks
   
;
exps:
  %empty
  | exp_or_null

exp_or_null:
  exp_or_null SEMI exp
  | exp

record_or_null:
  record_or_null COMMA ID EQ exp
  | ID EQ exp

record:
  %empty
  | record_or_null

function:
  %empty
  | function_or_null

function_or_null:
  function_or_null COMMA exp
  | exp

exp:
  /* Literals */
  NIL
  | INT
  | STRING
  /* Array and record creations. */
  | ID LBRACK exp RBRACK OF exp
  | ID LBRACE record RBRACE
  /* Variables, field, elements of an array. */
  | lvalue
  /* Function call. */
  | ID LPAREN function RPAREN
  /* Operations. */
  | MINUS exp %prec UMINUS
  | exp PLUS exp
  | exp MINUS exp
  | exp TIMES exp
  | exp DIVIDE exp
  | exp EQ exp
  | exp NE exp
  | exp GT exp
  | exp LT exp
  | exp GE exp
  | exp LE exp
  | exp AND exp
  | exp OR exp
  | LPAREN exps RPAREN
  /* Assignment. */
  | lvalue ASSIGN exp
  /* Control structures. */
  | IF exp THEN exp ELSE exp
  | IF exp THEN exp
  | WHILE exp DO exp
  | FOR ID ASSIGN exp TO exp DO exp
  | BREAK
  | LET chunks IN exps END
  | NEW typeid
  | lvalue DOT ID LPAREN function RPAREN
  ;

lvalue:
  ID
  | lvalue_b
  ;

lvalue_b:
  ID LBRACK exp RBRACK
  | ID DOT ID
  | lvalue_b LBRACK exp RBRACK
  | lvalue_b DOT ID

/*---------------.
| Declarations.  |
`---------------*/

%token CHUNKS "_chunks";
chunks:
  /* Chunks are contiguous series of declarations of the same type
     (TypeDec, FunctionDec...) to which we allow certain specfic behavior like
     self referencing.
     They are held by a ChunkList, that can be empty, like in this case:
        let
        in
            ..
        end
     which is why we end the recursion with a %empty. */
  %empty                  
| tychunk   chunks
| funchunk chunks
| vardec
| IMPORT STRING
;

/*----------------------.
| Variable Declarations.|
`----------------------*/

vardec:
  VAR ID type_id ASSIGN exp

/*----------------------.
| Function Declarations.|
`---------------------*/

funchunk:
  fundec %prec CHUNKS
  | fundec funchunk
  ;

fundec:
  FUNCTION ID LPAREN tyfields RPAREN type_id EQ exp
  | PRIMITIVE ID LPAREN tyfields RPAREN type_id
  ;

type_id:
  %empty
  | COLON typeid
  ;

/*--------------------.
| Type Declarations.  |
`--------------------*/

tychunk:
  /* Use `%prec CHUNKS' to do context-dependent precedence and resolve a
     shift-reduce conflict. */
  tydec %prec CHUNKS  
| tydec tychunk       
;

tydec:
  "type" ID "=" ty
  | CLASS ID extends LBRACE classfields RBRACE
;

extends:
  %empty
  | EXTENDS typeid
  ;

ty:
  typeid               
| "{" tyfields "}"     
| "array" "of" typeid
| CLASS extends LBRACE classfields RBRACE
;

classfields:
  %empty
  | classfields METHOD ID LPAREN tyfields RPAREN colonID EQ exp
  | classfields vardec
  ;

colonID:
  %empty
  | COLON typeid
  ;

tyfields:
  %empty               
| tyfields.1           
;

tyfields.1:
  tyfields.1 "," tyfield 
| tyfield                
;

tyfield:
  ID ":" typeid     
;

%token NAMETY "_namety";
typeid:
  ID                    
  /* This is a metavariable. It it used internally by TWEASTs to retrieve
     already parsed nodes when given an input to parse. */
| NAMETY "(" INT ")"    
;

%%

void
parse::parser::error(const location_type& l, const std::string& m)
{
  // FIXME: Some code was deleted here.
}
