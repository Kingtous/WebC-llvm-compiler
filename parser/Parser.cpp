/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 1 "Parser.y"

  #include <stdio.h>

#line 71 "Parser.cpp"



/* First part of user prologue.  */
#line 13 "Parser.y"

#include "Lexer.h"
extern char *yytext;
#ifdef CGUI
extern void logOnUi(const char* s);
#endif
void yyerror(const char *s)
{ fprintf(stderr,"%s on line %d, Col %d\n",s,TheLexer->getCLineNumber(),TheLexer->getCCol());
#ifdef CGUI
logOnUi((string(s)+" 在 " + to_string(TheLexer->getCLineNumber())+" 行 "+to_string(TheLexer->getCCol())+" 列").c_str());
#endif
}

#line 90 "Parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 5 "Parser.y"

#include "Lexer.h"
 #include "ast/NodeAST.hpp"
// 程序分析入口点
extern BlockAST* program;
using std::vector;

#line 141 "Parser.cpp"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END = 0,
    T_IDENTIFIER = 258,
    T_INTEGER = 259,
    T_DOUBLE = 260,
    T_ADD = 261,
    T_SUB = 262,
    T_MUL = 263,
    T_DIV = 264,
    T_MOD = 265,
    T_ASSIGN = 266,
    T_EQU = 267,
    T_N_EQU = 268,
    T_LESS = 269,
    T_GREATER = 270,
    T_REVERSE = 271,
    T_LESS_EQU = 272,
    T_GREATER_EQU = 273,
    T_COMMA = 274,
    T_SEMICOLON = 275,
    T_L_SPAR = 276,
    T_R_SPAR = 277,
    T_L_MPAR = 278,
    T_R_MPAR = 279,
    T_L_LPAR = 280,
    T_R_LPAR = 281,
    T_CONST = 282,
    T_STR = 283,
    T_NULL = 284,
    T_FOR = 285,
    T_WHILE = 286,
    T_OUT = 287,
    T_CONTINUE = 288,
    T_IF = 289,
    T_ELSE = 290,
    T_RETURN = 291,
    T_VOID = 292,
    T_INT = 293,
    T_FUNC_SIGN = 294,
    T_OR = 296,
    T_AND = 297,
    T_MINUS = 298,
    T_POS = 299
  };
#endif
/* Tokens.  */
#define END 0
#define T_IDENTIFIER 258
#define T_INTEGER 259
#define T_DOUBLE 260
#define T_ADD 261
#define T_SUB 262
#define T_MUL 263
#define T_DIV 264
#define T_MOD 265
#define T_ASSIGN 266
#define T_EQU 267
#define T_N_EQU 268
#define T_LESS 269
#define T_GREATER 270
#define T_REVERSE 271
#define T_LESS_EQU 272
#define T_GREATER_EQU 273
#define T_COMMA 274
#define T_SEMICOLON 275
#define T_L_SPAR 276
#define T_R_SPAR 277
#define T_L_MPAR 278
#define T_R_MPAR 279
#define T_L_LPAR 280
#define T_R_LPAR 281
#define T_CONST 282
#define T_STR 283
#define T_NULL 284
#define T_FOR 285
#define T_WHILE 286
#define T_OUT 287
#define T_CONTINUE 288
#define T_IF 289
#define T_ELSE 290
#define T_RETURN 291
#define T_VOID 292
#define T_INT 293
#define T_FUNC_SIGN 294
#define T_OR 296
#define T_AND 297
#define T_MINUS 298
#define T_POS 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 28 "Parser.y"

	NodeAST* node;
	ExpressionAST* expr;
	StatementAST* stmt;
	BlockAST* block;
	IdentifierExprAST* ident;
	IdentifierArrExprAST* identarr;
	VariableDeclarationAST* vdeclar;
	ConditionAST* cond;
	ForExprAST* forexpr;
	WhileStmtAST* whilestmt;
	FuncPtrAST* funcptr;
	std::vector<VariableDeclarationAST*> *varvec;
	std::vector<IdentifierExprAST*> *identvec;
	std::vector<ExpressionAST*> *exprvec;
	std::vector<ExpressionAST*> *aivec; // array index vector
	vector<NodeAST*>* arrayvalvec;
	std::string *string;
	double double_value;
	int int_value;
	int token;

#line 263 "Parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   520

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  149

#define YYUNDEFTOK  2
#define YYMAXUTOK   299


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    45,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   104,   104,   112,   113,   116,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     137,   141,   145,   151,   152,   158,   159,   162,   164,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     192,   193,   194,   195,   197,   198,   199,   202,   206,   211,
     212,   215,   223,   224,   226,   229,   230,   233,   238,   239,
     240,   243,   246,   247,   248,   251,   252,   254
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"END OF FILE BY SYSYPLUS COMPILER BY KINGTOUS\"", "error",
  "$undefined", "T_IDENTIFIER", "T_INTEGER", "T_DOUBLE", "T_ADD", "T_SUB",
  "T_MUL", "T_DIV", "T_MOD", "T_ASSIGN", "T_EQU", "T_N_EQU", "T_LESS",
  "T_GREATER", "T_REVERSE", "T_LESS_EQU", "T_GREATER_EQU", "T_COMMA",
  "T_SEMICOLON", "T_L_SPAR", "T_R_SPAR", "T_L_MPAR", "T_R_MPAR",
  "T_L_LPAR", "T_R_LPAR", "T_CONST", "T_STR", "T_NULL", "T_FOR", "T_WHILE",
  "T_OUT", "T_CONTINUE", "T_IF", "T_ELSE", "T_RETURN", "T_VOID", "T_INT",
  "T_FUNC_SIGN", "\"array index vector\"", "T_OR", "T_AND", "T_MINUS",
  "T_POS", "';'", "$accept", "program", "stmts", "stmt", "ident",
  "ident_arr", "var_decl", "func_ptr_args", "expr", "array_index",
  "call_args", "number", "block", "func_decl", "func_args", "if_condition",
  "for_stmt", "for_args", "while_stmt", "array_init_val",
  "array_init_list", "str", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,    59
};
# endif

#define YYPACT_NINF (-120)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     230,  -120,  -120,  -120,    10,   478,   156,     5,  -120,  -120,
      -6,     3,    -9,     1,    35,    13,    54,  -120,    63,   230,
    -120,    22,    53,    24,   198,  -120,  -120,  -120,  -120,  -120,
    -120,  -120,  -120,    86,  -120,   330,  -120,   193,    10,    61,
     445,   478,  -120,  -120,   478,  -120,   264,    10,  -120,  -120,
     478,   478,   146,    49,    66,  -120,   478,  -120,   478,   478,
     478,   478,   478,   478,   478,   478,   478,   478,   478,   478,
     478,  -120,  -120,  -120,    -1,  -120,    50,  -120,   423,    68,
     367,   384,  -120,  -120,   -10,   277,   423,    -8,    91,   404,
     478,     5,   465,   317,    79,    79,  -120,  -120,  -120,    73,
      73,    73,    73,    73,    73,   122,   502,   478,   445,    90,
      90,    10,    10,  -120,   478,  -120,  -120,    91,   423,  -120,
      32,   450,   423,  -120,  -120,   423,    71,  -120,    82,  -120,
    -120,   423,  -120,     5,    90,  -120,  -120,    36,   445,    90,
    -120,  -120,   465,  -120,    96,  -120,  -120,    90,  -120
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    20,    58,    57,     0,     0,     0,     0,    77,    49,
       0,     0,     0,     0,     0,     0,     0,    18,     0,     2,
       3,    30,    31,     0,     0,    33,    17,     7,     8,    14,
       9,    48,    32,    30,    31,     0,    60,     0,     0,     0,
       0,     0,    15,    16,     0,    12,     0,     0,     1,     4,
       0,    54,     0,    22,    24,    21,     0,     5,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    19,    34,    59,    22,     6,    30,    68,    70,     0,
       0,     0,    13,    27,     0,     0,    55,     0,    50,     0,
       0,    62,     0,     0,    35,    36,    37,    38,    39,    44,
      45,    40,    41,    42,    43,    47,    46,     0,     0,     0,
       0,     0,     0,    10,     0,    29,    52,    51,    23,    63,
       0,     0,    72,    25,    11,    69,     0,    71,    65,    26,
      28,    56,    53,     0,     0,    74,    76,     0,     0,     0,
      64,    61,     0,    73,     0,    66,    75,     0,    67
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -120,  -120,   113,    -7,     0,    29,    19,  -120,    34,   -85,
    -120,  -120,   -82,  -120,  -120,  -120,  -120,  -107,  -120,  -119,
    -120,  -120
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    18,    19,    20,    33,    34,    23,    84,    24,    55,
      87,    25,    26,    27,   120,    28,    29,    79,    30,   123,
     137,    31
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      21,   126,   136,   116,    32,   111,    21,    38,     1,   112,
      90,   114,    49,     1,   115,    40,     1,     2,     3,    21,
       4,    53,    52,   146,    41,     1,    39,   127,   128,    22,
      49,   144,   132,    50,     5,    22,    42,    21,    74,    35,
      76,     8,     9,    51,    16,    52,    43,    83,    22,    46,
      54,   133,   141,     1,   134,   142,    44,   145,    45,    77,
      90,   107,   143,    48,    56,   148,    22,    54,    47,    57,
      91,    51,    52,    52,    78,    80,    74,    92,    81,    58,
      59,    60,    61,    62,    85,    86,    89,    60,    61,    62,
      93,    38,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,    54,    75,    51,    76,    52,
     119,   129,   130,   108,    52,     6,   138,   139,   147,    37,
       0,     0,     0,     0,   118,     0,   122,    77,    58,    59,
      60,    61,    62,    38,    63,    64,    65,    66,    76,    67,
      68,   125,    78,     0,     0,     0,     0,     0,   131,     1,
       2,     3,   140,     4,     0,   122,     0,    77,     0,     1,
       2,     3,     0,     4,    70,     0,     0,     5,     0,     0,
      88,     0,    78,     0,     8,     9,   122,     5,     0,     0,
       0,     6,    36,     7,     8,     9,    10,    11,    12,    13,
      14,     0,    15,     0,     0,    16,     1,     2,     3,     0,
       4,    17,     0,     0,    58,    59,    60,    61,    62,     0,
      63,    64,    65,    66,     5,    67,    68,     0,     6,    73,
       7,     8,     9,    10,    11,    12,    13,    14,     0,    15,
       0,     0,    16,     1,     2,     3,     0,     4,    17,    69,
      70,     0,     0,    71,     0,     0,     0,     0,     0,     0,
       0,     5,     0,     0,     0,     6,     0,     7,     8,     9,
      10,    11,    12,    13,    14,     0,    15,     0,     0,    16,
      58,    59,    60,    61,    62,    17,    63,    64,    65,    66,
       0,    67,    68,    58,    59,    60,    61,    62,     0,    63,
      64,    65,    66,     0,    67,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,     0,     0,    82,
       0,     0,     0,     0,     0,     0,     0,     0,    69,    70,
       0,     0,   113,    58,    59,    60,    61,    62,     0,    63,
      64,    65,    66,     0,    67,    68,    58,    59,    60,    61,
      62,     0,    63,    64,    65,    66,     0,    67,    68,     0,
       0,     0,    72,     0,     0,     0,     0,     0,    69,    70,
       0,     0,   124,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    58,    59,    60,    61,    62,     0,    63,
      64,    65,    66,     0,    67,    68,     0,     0,     0,   109,
      58,    59,    60,    61,    62,     0,    63,    64,    65,    66,
       0,    67,    68,     0,     0,     0,   110,     0,    69,    70,
      58,    59,    60,    61,    62,     0,    63,    64,    65,    66,
       0,    67,    68,     0,     0,    69,    70,     0,   117,    58,
      59,    60,    61,    62,     0,    63,    64,    65,    66,     0,
      67,    68,     0,     0,     0,    69,    70,     0,     1,     2,
       3,     0,     4,     1,     2,     3,     0,     4,     0,     0,
       0,     0,     0,     0,    69,    70,     5,     0,     1,     2,
       3,     5,     4,     8,     9,   121,   135,     0,     8,     9,
       0,     1,     2,     3,    16,     4,     5,     0,     0,     0,
     121,     0,     0,     8,     9,     0,     0,     0,     0,     5,
       0,     0,     0,     0,     0,     0,     8,     9,    58,    59,
      60,    61,    62,     0,    63,    64,    65,    66,     0,    67,
      68
};

static const yytype_int16 yycheck[] =
{
       0,   108,   121,    88,     4,    15,     6,     7,     3,    19,
      11,    19,    19,     3,    22,    21,     3,     4,     5,    19,
       7,    21,    23,   142,    21,     3,     7,   109,   110,     0,
      37,   138,   117,    11,    21,     6,    45,    37,    38,     5,
      40,    28,    29,    21,    39,    23,    45,    47,    19,    15,
      21,    19,   134,     3,    22,    19,    21,   139,    45,    40,
      11,    11,    26,     0,    11,   147,    37,    38,    14,    45,
      21,    21,    23,    23,    40,    41,    76,    11,    44,     6,
       7,     8,     9,    10,    50,    51,    52,     8,     9,    10,
      56,    91,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    76,    45,    21,   108,    23,
      91,   111,   112,    45,    23,    25,    45,    35,    22,     6,
      -1,    -1,    -1,    -1,    90,    -1,    92,   108,     6,     7,
       8,     9,    10,   133,    12,    13,    14,    15,   138,    17,
      18,   107,   108,    -1,    -1,    -1,    -1,    -1,   114,     3,
       4,     5,   133,     7,    -1,   121,    -1,   138,    -1,     3,
       4,     5,    -1,     7,    42,    -1,    -1,    21,    -1,    -1,
      24,    -1,   138,    -1,    28,    29,   142,    21,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    36,    -1,    -1,    39,     3,     4,     5,    -1,
       7,    45,    -1,    -1,     6,     7,     8,     9,    10,    -1,
      12,    13,    14,    15,    21,    17,    18,    -1,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    36,
      -1,    -1,    39,     3,     4,     5,    -1,     7,    45,    41,
      42,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    25,    -1,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    36,    -1,    -1,    39,
       6,     7,     8,     9,    10,    45,    12,    13,    14,    15,
      -1,    17,    18,     6,     7,     8,     9,    10,    -1,    12,
      13,    14,    15,    -1,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    42,    -1,    -1,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    45,     6,     7,     8,     9,    10,    -1,    12,
      13,    14,    15,    -1,    17,    18,     6,     7,     8,     9,
      10,    -1,    12,    13,    14,    15,    -1,    17,    18,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,     6,     7,     8,     9,    10,    -1,    12,
      13,    14,    15,    -1,    17,    18,    -1,    -1,    -1,    22,
       6,     7,     8,     9,    10,    -1,    12,    13,    14,    15,
      -1,    17,    18,    -1,    -1,    -1,    22,    -1,    41,    42,
       6,     7,     8,     9,    10,    -1,    12,    13,    14,    15,
      -1,    17,    18,    -1,    -1,    41,    42,    -1,    24,     6,
       7,     8,     9,    10,    -1,    12,    13,    14,    15,    -1,
      17,    18,    -1,    -1,    -1,    41,    42,    -1,     3,     4,
       5,    -1,     7,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    42,    21,    -1,     3,     4,
       5,    21,     7,    28,    29,    25,    26,    -1,    28,    29,
      -1,     3,     4,     5,    39,     7,    21,    -1,    -1,    -1,
      25,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    -1,    -1,    28,    29,     6,     7,
       8,     9,    10,    -1,    12,    13,    14,    15,    -1,    17,
      18
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,    21,    25,    27,    28,    29,
      30,    31,    32,    33,    34,    36,    39,    45,    47,    48,
      49,    50,    51,    52,    54,    57,    58,    59,    61,    62,
      64,    67,    50,    50,    51,    54,    26,    48,    50,    52,
      21,    21,    45,    45,    21,    45,    54,    14,     0,    49,
      11,    21,    23,    50,    51,    55,    11,    45,     6,     7,
       8,     9,    10,    12,    13,    14,    15,    17,    18,    41,
      42,    45,    22,    26,    50,    45,    50,    52,    54,    63,
      54,    54,    45,    50,    53,    54,    54,    56,    24,    54,
      11,    21,    11,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    11,    45,    22,
      22,    15,    19,    45,    19,    22,    55,    24,    54,    52,
      60,    25,    54,    65,    45,    54,    63,    58,    58,    50,
      50,    54,    55,    19,    22,    26,    65,    66,    45,    35,
      52,    58,    19,    26,    63,    58,    65,    22,    58
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      50,    51,    52,    52,    52,    52,    52,    53,    53,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      55,    55,    55,    55,    56,    56,    56,    57,    57,    58,
      58,    59,    60,    60,    60,    61,    61,    62,    63,    63,
      63,    64,    65,    65,    65,    66,    66,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     3,     1,     1,     1,
       4,     4,     2,     3,     1,     2,     2,     1,     1,     2,
       1,     2,     2,     4,     2,     4,     5,     1,     3,     4,
       1,     1,     2,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       2,     3,     3,     4,     0,     1,     3,     1,     1,     3,
       2,     6,     0,     1,     3,     5,     7,     9,     1,     3,
       1,     5,     1,     3,     2,     3,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 104 "Parser.y"
               {
			program = (yyvsp[0].block);
			#ifdef DEBUG_FLAG
			fprintf(stdout,"parse success\n");
			#endif
		}
#line 1620 "Parser.cpp"
    break;

  case 3:
#line 112 "Parser.y"
             {(yyval.block) = new BlockAST(); (yyval.block)->statements.push_back((yyvsp[0].stmt));}
#line 1626 "Parser.cpp"
    break;

  case 4:
#line 113 "Parser.y"
                     {(yyvsp[-1].block)->statements.push_back((yyvsp[0].stmt));}
#line 1632 "Parser.cpp"
    break;

  case 5:
#line 116 "Parser.y"
                    {
#ifdef DEBUG_FLAG
fprintf(stderr,"build var decl stmt\n");
#endif
}
#line 1642 "Parser.cpp"
    break;

  case 6:
#line 121 "Parser.y"
                               {(yyvsp[-1].vdeclar)->setIsConst(true);(yyval.stmt)=(yyvsp[-1].vdeclar);}
#line 1648 "Parser.cpp"
    break;

  case 7:
#line 122 "Parser.y"
                    {(yyval.stmt) = (yyvsp[0].stmt);}
#line 1654 "Parser.cpp"
    break;

  case 8:
#line 123 "Parser.y"
                       {(yyval.stmt) = (yyvsp[0].cond);}
#line 1660 "Parser.cpp"
    break;

  case 9:
#line 124 "Parser.y"
                     {(yyval.stmt) = (yyvsp[0].whilestmt);}
#line 1666 "Parser.cpp"
    break;

  case 10:
#line 125 "Parser.y"
                                  {(yyval.stmt) = new VariableAssignmentAST((yyvsp[-3].ident)->identifier,(yyvsp[-1].expr));}
#line 1672 "Parser.cpp"
    break;

  case 11:
#line 126 "Parser.y"
                                      {(yyval.stmt) = new VariableArrAssignmentAST((yyvsp[-3].identarr),(yyvsp[-1].expr));}
#line 1678 "Parser.cpp"
    break;

  case 12:
#line 127 "Parser.y"
                      {(yyval.stmt) = new ReturnStmtAST();}
#line 1684 "Parser.cpp"
    break;

  case 13:
#line 128 "Parser.y"
                           {(yyval.stmt) = new ReturnStmtAST((yyvsp[-1].expr));}
#line 1690 "Parser.cpp"
    break;

  case 14:
#line 129 "Parser.y"
                   {(yyval.stmt) = (yyvsp[0].forexpr);}
#line 1696 "Parser.cpp"
    break;

  case 15:
#line 130 "Parser.y"
                    {(yyval.stmt) = new OutStmtAST();}
#line 1702 "Parser.cpp"
    break;

  case 16:
#line 131 "Parser.y"
                         {(yyval.stmt) = new ContinueStmtAST();}
#line 1708 "Parser.cpp"
    break;

  case 17:
#line 132 "Parser.y"
                {(yyval.stmt) = (yyvsp[0].block);}
#line 1714 "Parser.cpp"
    break;

  case 18:
#line 133 "Parser.y"
              {}
#line 1720 "Parser.cpp"
    break;

  case 19:
#line 134 "Parser.y"
                   {(yyval.stmt) = new ExpressionStatementAST((yyvsp[-1].expr));}
#line 1726 "Parser.cpp"
    break;

  case 20:
#line 137 "Parser.y"
                     {
 (yyval.ident) = new IdentifierExprAST(*(yyvsp[0].string));}
#line 1733 "Parser.cpp"
    break;

  case 21:
#line 141 "Parser.y"
                              { (yyval.identarr) = new IdentifierArrExprAST((yyvsp[-1].ident)->identifier,(yyvsp[0].aivec));}
#line 1739 "Parser.cpp"
    break;

  case 22:
#line 145 "Parser.y"
                       {
		(yyval.vdeclar) = new VariableDeclarationAST(
		(yyvsp[-1].ident)->identifier,
		(yyvsp[0].ident)
		);
		}
#line 1750 "Parser.cpp"
    break;

  case 23:
#line 151 "Parser.y"
                                    {(yyval.vdeclar) = new VariableDeclarationAST((yyvsp[-3].ident)->identifier,(yyvsp[-2].ident),(yyvsp[0].expr));}
#line 1756 "Parser.cpp"
    break;

  case 24:
#line 152 "Parser.y"
                          {
          		(yyval.vdeclar) = new VariableArrDeclarationAST(
          		(yyvsp[-1].ident)->identifier,
          		(yyvsp[0].identarr)
          		);
          		}
#line 1767 "Parser.cpp"
    break;

  case 25:
#line 158 "Parser.y"
                                                   {(yyval.vdeclar) = new VariableArrDeclarationAST((yyvsp[-3].ident)->identifier,(yyvsp[-2].identarr),(yyvsp[0].arrayvalvec));}
#line 1773 "Parser.cpp"
    break;

  case 26:
#line 159 "Parser.y"
                                                           {(yyval.vdeclar) = new FuncPtrAST(*(yyvsp[-2].identvec),(yyvsp[0].ident));}
#line 1779 "Parser.cpp"
    break;

  case 27:
#line 162 "Parser.y"
                    {(yyval.identvec) = new std::vector<IdentifierExprAST*>();
                   (yyval.identvec)->push_back((yyvsp[0].ident)); }
#line 1786 "Parser.cpp"
    break;

  case 28:
#line 164 "Parser.y"
                                    {(yyvsp[-2].identvec)->push_back((yyvsp[0].ident));}
#line 1792 "Parser.cpp"
    break;

  case 29:
#line 169 "Parser.y"
                                         {(yyval.expr) = new CallExprAST((yyvsp[-3].ident)->identifier,*(yyvsp[-1].exprvec)); }
#line 1798 "Parser.cpp"
    break;

  case 30:
#line 170 "Parser.y"
                {(yyval.ident) = (yyvsp[0].ident);}
#line 1804 "Parser.cpp"
    break;

  case 31:
#line 171 "Parser.y"
                    {(yyval.ident) = (yyvsp[0].identarr);}
#line 1810 "Parser.cpp"
    break;

  case 32:
#line 172 "Parser.y"
                      {(yyval.expr) = new BinaryExprAST(BinaryType::sub,new IntegerExprAST(0),(yyvsp[0].ident));}
#line 1816 "Parser.cpp"
    break;

  case 34:
#line 174 "Parser.y"
                                 {(yyval.expr) = (yyvsp[-1].expr);}
#line 1822 "Parser.cpp"
    break;

  case 35:
#line 175 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::add,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1828 "Parser.cpp"
    break;

  case 36:
#line 176 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::sub,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1834 "Parser.cpp"
    break;

  case 37:
#line 177 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::mul,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1840 "Parser.cpp"
    break;

  case 38:
#line 178 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::divi,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1846 "Parser.cpp"
    break;

  case 39:
#line 179 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::mod,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1852 "Parser.cpp"
    break;

  case 40:
#line 180 "Parser.y"
                           {(yyval.expr) = new BinaryExprAST(BinaryType::less,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1858 "Parser.cpp"
    break;

  case 41:
#line 181 "Parser.y"
                              {(yyval.expr) = new BinaryExprAST(BinaryType::greater,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1864 "Parser.cpp"
    break;

  case 42:
#line 182 "Parser.y"
                               {(yyval.expr) = new BinaryExprAST(BinaryType::less_equ,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1870 "Parser.cpp"
    break;

  case 43:
#line 183 "Parser.y"
                                  {(yyval.expr) = new BinaryExprAST(BinaryType::greater_equ,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1876 "Parser.cpp"
    break;

  case 44:
#line 184 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::equ,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1882 "Parser.cpp"
    break;

  case 45:
#line 185 "Parser.y"
                            {(yyval.expr) = new BinaryExprAST(BinaryType::n_equ,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1888 "Parser.cpp"
    break;

  case 46:
#line 186 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::AND,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1894 "Parser.cpp"
    break;

  case 47:
#line 187 "Parser.y"
                         {(yyval.expr) = new BinaryExprAST(BinaryType::OR,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1900 "Parser.cpp"
    break;

  case 48:
#line 188 "Parser.y"
              {(yyval.expr) = (yyvsp[0].expr);}
#line 1906 "Parser.cpp"
    break;

  case 49:
#line 189 "Parser.y"
                 {(yyval.expr) = new NullExprAST();}
#line 1912 "Parser.cpp"
    break;

  case 50:
#line 192 "Parser.y"
                                {(yyval.aivec) = new vector<ExpressionAST*>(); (yyval.aivec)->push_back(NIL);}
#line 1918 "Parser.cpp"
    break;

  case 51:
#line 193 "Parser.y"
                                 {(yyval.aivec) = new vector<ExpressionAST*>(); (yyval.aivec)->push_back((yyvsp[-1].expr));}
#line 1924 "Parser.cpp"
    break;

  case 52:
#line 194 "Parser.y"
                                       {(yyvsp[0].aivec)->insert((yyvsp[0].aivec)->begin(),NIL); (yyval.aivec) = (yyvsp[0].aivec);}
#line 1930 "Parser.cpp"
    break;

  case 53:
#line 195 "Parser.y"
                                             {(yyvsp[0].aivec)->insert((yyvsp[0].aivec)->begin(),(yyvsp[-2].expr)); (yyval.aivec) = (yyvsp[0].aivec);}
#line 1936 "Parser.cpp"
    break;

  case 54:
#line 197 "Parser.y"
                    {(yyval.exprvec) = new std::vector<ExpressionAST*>();}
#line 1942 "Parser.cpp"
    break;

  case 55:
#line 198 "Parser.y"
               {(yyval.exprvec) = new std::vector<ExpressionAST*>();(yyval.exprvec)->push_back((yyvsp[0].expr));}
#line 1948 "Parser.cpp"
    break;

  case 56:
#line 199 "Parser.y"
                                 {(yyvsp[-2].exprvec)->push_back((yyvsp[0].expr));}
#line 1954 "Parser.cpp"
    break;

  case 57:
#line 203 "Parser.y"
                {
                    (yyval.expr) = new DoubleExprAST((yyvsp[0].double_value));
                }
#line 1962 "Parser.cpp"
    break;

  case 58:
#line 206 "Parser.y"
                   {
       		(yyval.expr) = new IntegerExprAST((yyvsp[0].int_value));
       }
#line 1970 "Parser.cpp"
    break;

  case 59:
#line 211 "Parser.y"
                                {(yyval.block) = (yyvsp[-1].block);}
#line 1976 "Parser.cpp"
    break;

  case 60:
#line 212 "Parser.y"
                            {(yyval.block) = new BlockAST();}
#line 1982 "Parser.cpp"
    break;

  case 61:
#line 216 "Parser.y"
{
	PrototypeAST* proto = new PrototypeAST((yyvsp[-5].ident)->identifier,(yyvsp[-4].ident)->identifier,*(yyvsp[-2].varvec));
	BlockAST* body = (yyvsp[0].block);
	(yyval.stmt) = new FunctionAST(proto,body);
	//printf("build function %s \n",$2->identifier.c_str());
}
#line 1993 "Parser.cpp"
    break;

  case 62:
#line 223 "Parser.y"
                   { (yyval.varvec) = new std::vector<VariableDeclarationAST*>(); }
#line 1999 "Parser.cpp"
    break;

  case 63:
#line 224 "Parser.y"
                 { (yyval.varvec) = new std::vector<VariableDeclarationAST*>();
                   (yyval.varvec)->push_back((yyvsp[0].vdeclar)); }
#line 2006 "Parser.cpp"
    break;

  case 64:
#line 226 "Parser.y"
                                   { (yyvsp[-2].varvec)->push_back((yyvsp[0].vdeclar)); }
#line 2012 "Parser.cpp"
    break;

  case 65:
#line 229 "Parser.y"
                                                 {(yyval.cond) = new ConditionAST((yyvsp[-2].expr),(yyvsp[0].block),nullptr);}
#line 2018 "Parser.cpp"
    break;

  case 66:
#line 230 "Parser.y"
                                                         {(yyval.cond) = new ConditionAST((yyvsp[-4].expr),(yyvsp[-2].block),(yyvsp[0].block));}
#line 2024 "Parser.cpp"
    break;

  case 67:
#line 234 "Parser.y"
        {
		(yyval.forexpr) = new ForExprAST((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].block));
	}
#line 2032 "Parser.cpp"
    break;

  case 68:
#line 238 "Parser.y"
                    {(yyval.node) = (yyvsp[0].vdeclar);}
#line 2038 "Parser.cpp"
    break;

  case 69:
#line 239 "Parser.y"
                              {(yyval.node) = new VariableAssignmentAST((yyvsp[-2].ident)->identifier,(yyvsp[0].expr));}
#line 2044 "Parser.cpp"
    break;

  case 70:
#line 240 "Parser.y"
               {(yyval.node) = (yyvsp[0].expr);}
#line 2050 "Parser.cpp"
    break;

  case 71:
#line 243 "Parser.y"
                                                  {(yyval.whilestmt) = new WhileStmtAST((yyvsp[-2].expr),(yyvsp[0].block));}
#line 2056 "Parser.cpp"
    break;

  case 72:
#line 246 "Parser.y"
                       {(yyval.arrayvalvec) = new vector<NodeAST*>(); (yyval.arrayvalvec)->push_back((yyvsp[0].expr));}
#line 2062 "Parser.cpp"
    break;

  case 73:
#line 247 "Parser.y"
                                            {(yyval.arrayvalvec) = (yyvsp[-1].arrayvalvec);}
#line 2068 "Parser.cpp"
    break;

  case 74:
#line 248 "Parser.y"
                            {(yyval.arrayvalvec) = new vector<NodeAST*>();}
#line 2074 "Parser.cpp"
    break;

  case 75:
#line 251 "Parser.y"
                                                         {(yyval.arrayvalvec) = (yyvsp[-2].arrayvalvec); (yyvsp[-2].arrayvalvec)->insert((yyvsp[-2].arrayvalvec)->end(),(yyvsp[0].arrayvalvec)->begin(),(yyvsp[0].arrayvalvec)->end());}
#line 2080 "Parser.cpp"
    break;

  case 76:
#line 252 "Parser.y"
                         {(yyval.arrayvalvec) = (yyvsp[0].arrayvalvec);}
#line 2086 "Parser.cpp"
    break;

  case 77:
#line 254 "Parser.y"
            {(yyval.expr) = new StringExprAST((yyval.string));}
#line 2092 "Parser.cpp"
    break;


#line 2096 "Parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 257 "Parser.y"


BlockAST* program = nullptr;

BlockAST* run_parser(std::string path) {
    FILE* yyin = fopen(path.c_str(), "r+");
    program = nullptr;
    int ret = yyparse();
    if (ret != 0)
        program = nullptr;
    fclose(yyin);
    return program;
}

