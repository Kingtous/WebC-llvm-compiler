/* A Bison parser, made by GNU Bison 3.7.4.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30704

/* Bison version string.  */
#define YYBISON_VERSION "3.7.4"

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

  #define _GNU_SOURCE
  #include <stdio.h>

#line 73 "Parser.cpp"



/* First part of user prologue.  */
#line 14 "Parser.y"

#include "Lexer.h"
extern char *yytext;
void yyerror(const char *s)
{ fprintf(stderr,"%s on line %d, Col %d\n",s,TheLexer->getCLineNumber(),TheLexer->getCCol()); }

#line 85 "Parser.cpp"

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

#include "Parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "END OF FILE"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_IDENTIFIER = 3,               /* T_IDENTIFIER  */
  YYSYMBOL_T_INTEGER = 4,                  /* T_INTEGER  */
  YYSYMBOL_T_DOUBLE = 5,                   /* T_DOUBLE  */
  YYSYMBOL_T_ADD = 6,                      /* T_ADD  */
  YYSYMBOL_T_SUB = 7,                      /* T_SUB  */
  YYSYMBOL_T_MUL = 8,                      /* T_MUL  */
  YYSYMBOL_T_DIV = 9,                      /* T_DIV  */
  YYSYMBOL_T_MOD = 10,                     /* T_MOD  */
  YYSYMBOL_T_ASSIGN = 11,                  /* T_ASSIGN  */
  YYSYMBOL_T_EQU = 12,                     /* T_EQU  */
  YYSYMBOL_T_N_EQU = 13,                   /* T_N_EQU  */
  YYSYMBOL_T_LESS = 14,                    /* T_LESS  */
  YYSYMBOL_T_GREATER = 15,                 /* T_GREATER  */
  YYSYMBOL_T_REVERSE = 16,                 /* T_REVERSE  */
  YYSYMBOL_T_LESS_EQU = 17,                /* T_LESS_EQU  */
  YYSYMBOL_T_GREATER_EQU = 18,             /* T_GREATER_EQU  */
  YYSYMBOL_T_COMMA = 19,                   /* T_COMMA  */
  YYSYMBOL_T_SEMICOLON = 20,               /* T_SEMICOLON  */
  YYSYMBOL_T_L_SPAR = 21,                  /* T_L_SPAR  */
  YYSYMBOL_T_R_SPAR = 22,                  /* T_R_SPAR  */
  YYSYMBOL_T_L_MPAR = 23,                  /* T_L_MPAR  */
  YYSYMBOL_T_R_MPAR = 24,                  /* T_R_MPAR  */
  YYSYMBOL_T_L_LPAR = 25,                  /* T_L_LPAR  */
  YYSYMBOL_T_R_LPAR = 26,                  /* T_R_LPAR  */
  YYSYMBOL_T_FOR = 27,                     /* T_FOR  */
  YYSYMBOL_T_WHILE = 28,                   /* T_WHILE  */
  YYSYMBOL_T_BREAK = 29,                   /* T_BREAK  */
  YYSYMBOL_T_CONTINUE = 30,                /* T_CONTINUE  */
  YYSYMBOL_T_IF = 31,                      /* T_IF  */
  YYSYMBOL_T_ELSE = 32,                    /* T_ELSE  */
  YYSYMBOL_T_RETURN = 33,                  /* T_RETURN  */
  YYSYMBOL_T_VOID = 34,                    /* T_VOID  */
  YYSYMBOL_T_INT = 35,                     /* T_INT  */
  YYSYMBOL_T_OR = 36,                      /* T_OR  */
  YYSYMBOL_T_AND = 37,                     /* T_AND  */
  YYSYMBOL_T_MINUS = 38,                   /* T_MINUS  */
  YYSYMBOL_T_POS = 39,                     /* T_POS  */
  YYSYMBOL_40_ = 40,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_program = 42,                   /* program  */
  YYSYMBOL_stmts = 43,                     /* stmts  */
  YYSYMBOL_stmt = 44,                      /* stmt  */
  YYSYMBOL_ident = 45,                     /* ident  */
  YYSYMBOL_var_decl = 46,                  /* var_decl  */
  YYSYMBOL_expr = 47,                      /* expr  */
  YYSYMBOL_call_args = 48,                 /* call_args  */
  YYSYMBOL_number = 49,                    /* number  */
  YYSYMBOL_block = 50,                     /* block  */
  YYSYMBOL_func_decl = 51,                 /* func_decl  */
  YYSYMBOL_func_args = 52,                 /* func_args  */
  YYSYMBOL_if_condition = 53,              /* if_condition  */
  YYSYMBOL_for_stmt = 54,                  /* for_stmt  */
  YYSYMBOL_for_args = 55                   /* for_args  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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
typedef yytype_int8 yy_state_t;

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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  24
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   188

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  47
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  93

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
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
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    83,    83,    89,    90,    93,    94,    95,    96,    97,
      98,    99,   100,   103,   110,   116,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   140,   141,   142,   146,   150,   155,   156,   159,   167,
     168,   170,   173,   174,   177,   182,   183,   184
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"END OF FILE\"", "error", "\"invalid token\"", "T_IDENTIFIER",
  "T_INTEGER", "T_DOUBLE", "T_ADD", "T_SUB", "T_MUL", "T_DIV", "T_MOD",
  "T_ASSIGN", "T_EQU", "T_N_EQU", "T_LESS", "T_GREATER", "T_REVERSE",
  "T_LESS_EQU", "T_GREATER_EQU", "T_COMMA", "T_SEMICOLON", "T_L_SPAR",
  "T_R_SPAR", "T_L_MPAR", "T_R_MPAR", "T_L_LPAR", "T_R_LPAR", "T_FOR",
  "T_WHILE", "T_BREAK", "T_CONTINUE", "T_IF", "T_ELSE", "T_RETURN",
  "T_VOID", "T_INT", "T_OR", "T_AND", "T_MINUS", "T_POS", "';'", "$accept",
  "program", "stmts", "stmt", "ident", "var_decl", "expr", "call_args",
  "number", "block", "func_decl", "func_args", "if_condition", "for_stmt",
  "for_args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      59
};
#endif

#define YYPACT_NINF (-77)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      71,   -77,   -11,    -9,    44,   -77,    16,    71,   -77,     8,
     -20,   -77,   -77,   -77,    31,    31,   -77,   -77,    25,    31,
     -77,     3,   100,   -77,   -77,   -77,    31,     6,   -77,     2,
     -77,   165,   -19,   127,   -77,   148,    31,    31,    31,    31,
      31,    31,    31,    31,    31,    31,    31,   -77,   113,    31,
      25,    31,    30,    31,    43,   -77,   165,    21,    83,    83,
     -77,   -77,   -77,   178,   178,   178,   178,   178,   -77,   165,
      25,   -77,    66,   165,    32,    -1,    14,    31,   -77,    25,
      43,    31,   -77,    63,    43,   165,   -77,   -77,    54,   -77,
     -77,    43,   -77
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    13,     0,     0,     0,    12,     0,     2,     3,     0,
       0,     6,     7,    11,     0,     0,    35,    34,     0,     0,
       9,    17,     0,    19,     1,     4,     0,    14,     5,    17,
      45,    47,     0,     0,    18,     0,    31,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,     0,     0,
      39,     0,    14,     0,     0,    20,    32,     0,    21,    22,
      23,    24,    25,    30,    26,    27,    28,    29,     8,    15,
       0,    40,     0,    46,     0,     0,    42,     0,    16,     0,
       0,     0,    37,     0,     0,    33,    41,    38,     0,    36,
      43,     0,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -77,   -77,     5,    -6,     0,    -8,    18,   -77,   -77,   -76,
     -77,   -77,   -77,   -77,   -50
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     6,     7,     8,    21,    10,    31,    57,    23,    76,
      11,    72,    12,    13,    32
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       9,    25,     1,    74,    87,     1,    30,     9,    90,    27,
      14,     1,    15,    51,    29,    92,    24,    49,    34,    26,
      28,    53,    22,    36,    36,    82,     2,    50,     1,    52,
       3,    88,     4,    33,     1,    16,    17,    35,    18,     5,
      77,    49,    71,    78,    48,    30,    84,     1,    16,    17,
      70,    18,    19,    29,    56,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    19,     1,    69,    75,    73,
      52,    86,    81,    30,     1,     9,    91,    25,     0,    70,
      83,    29,     0,     9,    20,    79,     0,     0,    80,    89,
       2,    39,    40,    41,     3,    85,     4,     0,     2,     0,
       0,     0,     3,     5,     4,     0,    37,    38,    39,    40,
      41,     5,    42,     0,    43,    44,     0,    45,    46,    37,
      38,    39,    40,    41,     0,    42,     0,    43,    44,     0,
      45,    46,     0,    37,    38,    39,    40,    41,     0,    42,
      47,    43,    44,     0,    45,    46,     0,     0,     0,    54,
       0,     0,     0,    68,    37,    38,    39,    40,    41,     0,
      42,     0,    43,    44,     0,    45,    46,     0,     0,     0,
      55,    37,    38,    39,    40,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    37,    38,    39,    40,    41
};

static const yytype_int8 yycheck[] =
{
       0,     7,     3,    53,    80,     3,    14,     7,    84,     9,
      21,     3,    21,    11,    14,    91,     0,    11,    18,    11,
      40,    40,     4,    21,    21,    26,    27,    21,     3,    29,
      31,    81,    33,    15,     3,     4,     5,    19,     7,    40,
      19,    11,    50,    22,    26,    53,    32,     3,     4,     5,
      50,     7,    21,    53,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    21,     3,    49,    25,    51,
      70,    79,    40,    81,     3,    75,    22,    83,    -1,    79,
      75,    81,    -1,    83,    40,    19,    -1,    -1,    22,    26,
      27,     8,     9,    10,    31,    77,    33,    -1,    27,    -1,
      -1,    -1,    31,    40,    33,    -1,     6,     7,     8,     9,
      10,    40,    12,    -1,    14,    15,    -1,    17,    18,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    -1,
      17,    18,    -1,     6,     7,     8,     9,    10,    -1,    12,
      40,    14,    15,    -1,    17,    18,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    40,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    -1,    17,    18,    -1,    -1,    -1,
      22,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      15,    -1,    17,    18,     6,     7,     8,     9,    10
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    27,    31,    33,    40,    42,    43,    44,    45,
      46,    51,    53,    54,    21,    21,     4,     5,     7,    21,
      40,    45,    47,    49,     0,    44,    11,    45,    40,    45,
      46,    47,    55,    47,    45,    47,    21,     6,     7,     8,
       9,    10,    12,    14,    15,    17,    18,    40,    47,    11,
      21,    11,    45,    40,    22,    22,    47,    48,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    40,    47,
      45,    46,    52,    47,    55,    25,    50,    19,    22,    19,
      22,    40,    26,    43,    32,    47,    46,    50,    55,    26,
      50,    22,    50
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    44,    44,    44,
      44,    44,    44,    45,    46,    46,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    48,    48,    48,    49,    49,    50,    50,    51,    52,
      52,    52,    53,    53,    54,    55,    55,    55
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     4,     2,
       3,     1,     1,     1,     2,     4,     4,     1,     2,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     0,     1,     3,     1,     1,     3,     2,     6,     0,
       1,     3,     5,     7,     9,     1,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

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
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= END)
    {
      yychar = END;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* program: stmts  */
#line 83 "Parser.y"
               {
			program = (yyvsp[0].block);
			fprintf(stdout,"parse success\n");
		}
#line 1204 "Parser.cpp"
    break;

  case 3: /* stmts: stmt  */
#line 89 "Parser.y"
             {(yyval.block) = new BlockAST(); (yyval.block)->statements.push_back((yyvsp[0].stmt));}
#line 1210 "Parser.cpp"
    break;

  case 4: /* stmts: stmts stmt  */
#line 90 "Parser.y"
                     {(yyvsp[-1].block)->statements.push_back((yyvsp[0].stmt));}
#line 1216 "Parser.cpp"
    break;

  case 5: /* stmt: var_decl ';'  */
#line 93 "Parser.y"
                    {printf("build var decl stmt\n");}
#line 1222 "Parser.cpp"
    break;

  case 6: /* stmt: func_decl  */
#line 94 "Parser.y"
                    {(yyval.stmt) = (yyvsp[0].stmt);}
#line 1228 "Parser.cpp"
    break;

  case 7: /* stmt: if_condition  */
#line 95 "Parser.y"
                       {(yyval.stmt) = (yyvsp[0].cond);}
#line 1234 "Parser.cpp"
    break;

  case 8: /* stmt: ident T_ASSIGN expr ';'  */
#line 96 "Parser.y"
                                  {(yyval.stmt) = new VariableAssignmentAST((yyvsp[-3].ident)->identifier,(yyvsp[-1].expr));}
#line 1240 "Parser.cpp"
    break;

  case 9: /* stmt: T_RETURN ';'  */
#line 97 "Parser.y"
                      {(yyval.stmt) = new ReturnStmtAST();}
#line 1246 "Parser.cpp"
    break;

  case 10: /* stmt: T_RETURN expr ';'  */
#line 98 "Parser.y"
                           {(yyval.stmt) = new ReturnStmtAST((yyvsp[-1].expr));}
#line 1252 "Parser.cpp"
    break;

  case 11: /* stmt: for_stmt  */
#line 99 "Parser.y"
                   {(yyval.stmt) = (yyvsp[0].forexpr);}
#line 1258 "Parser.cpp"
    break;

  case 12: /* stmt: ';'  */
#line 100 "Parser.y"
              {}
#line 1264 "Parser.cpp"
    break;

  case 13: /* ident: T_IDENTIFIER  */
#line 103 "Parser.y"
                     {
//std::fprintf(stderr,"build identifier: %s\n", $1->c_str());
 (yyval.ident) = new IdentifierExprAST(*(yyvsp[0].string));}
#line 1272 "Parser.cpp"
    break;

  case 14: /* var_decl: ident ident  */
#line 110 "Parser.y"
                       {
		(yyval.stmt) = new VariableDeclarationAST(
		(yyvsp[-1].ident)->identifier,
		(yyvsp[0].ident)->identifier
		);
		}
#line 1283 "Parser.cpp"
    break;

  case 15: /* var_decl: ident ident T_ASSIGN expr  */
#line 116 "Parser.y"
                                    {(yyval.stmt) = new VariableDeclarationAST((yyvsp[-3].ident)->identifier,(yyvsp[-2].ident)->identifier,(yyvsp[0].expr));}
#line 1289 "Parser.cpp"
    break;

  case 16: /* expr: ident T_L_SPAR call_args T_R_SPAR  */
#line 123 "Parser.y"
                                         {(yyval.expr) = new CallExprAST((yyvsp[-3].ident)->identifier,*(yyvsp[-1].exprvec)); }
#line 1295 "Parser.cpp"
    break;

  case 17: /* expr: ident  */
#line 124 "Parser.y"
                {(yyval.ident) = (yyvsp[0].ident);}
#line 1301 "Parser.cpp"
    break;

  case 18: /* expr: T_SUB ident  */
#line 125 "Parser.y"
                      {(yyval.expr) = new BinaryExprAST(BinaryType::sub,new IntegerExprAST(0),(yyvsp[0].ident));}
#line 1307 "Parser.cpp"
    break;

  case 20: /* expr: T_L_SPAR expr T_R_SPAR  */
#line 127 "Parser.y"
                                 {(yyval.expr) = (yyvsp[-1].expr); fprintf(stderr,"build (expr).\n");}
#line 1313 "Parser.cpp"
    break;

  case 21: /* expr: expr T_ADD expr  */
#line 128 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::add,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1319 "Parser.cpp"
    break;

  case 22: /* expr: expr T_SUB expr  */
#line 129 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::sub,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1325 "Parser.cpp"
    break;

  case 23: /* expr: expr T_MUL expr  */
#line 130 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::mul,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1331 "Parser.cpp"
    break;

  case 24: /* expr: expr T_DIV expr  */
#line 131 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::div,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1337 "Parser.cpp"
    break;

  case 25: /* expr: expr T_MOD expr  */
#line 132 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::mod,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1343 "Parser.cpp"
    break;

  case 26: /* expr: expr T_LESS expr  */
#line 133 "Parser.y"
                           {(yyval.expr) = new BinaryExprAST(BinaryType::less,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1349 "Parser.cpp"
    break;

  case 27: /* expr: expr T_GREATER expr  */
#line 134 "Parser.y"
                              {(yyval.expr) = new BinaryExprAST(BinaryType::greater,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1355 "Parser.cpp"
    break;

  case 28: /* expr: expr T_LESS_EQU expr  */
#line 135 "Parser.y"
                               {(yyval.expr) = new BinaryExprAST(BinaryType::less_equ,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1361 "Parser.cpp"
    break;

  case 29: /* expr: expr T_GREATER_EQU expr  */
#line 136 "Parser.y"
                                  {(yyval.expr) = new BinaryExprAST(BinaryType::greater_equ,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1367 "Parser.cpp"
    break;

  case 30: /* expr: expr T_EQU expr  */
#line 137 "Parser.y"
                          {(yyval.expr) = new BinaryExprAST(BinaryType::equ,(yyvsp[-2].expr),(yyvsp[0].expr));}
#line 1373 "Parser.cpp"
    break;

  case 31: /* call_args: %empty  */
#line 140 "Parser.y"
             {(yyval.exprvec) = new std::vector<ExpressionAST*>();}
#line 1379 "Parser.cpp"
    break;

  case 32: /* call_args: expr  */
#line 141 "Parser.y"
               {(yyval.exprvec) = new std::vector<ExpressionAST*>();(yyval.exprvec)->push_back((yyvsp[0].expr));}
#line 1385 "Parser.cpp"
    break;

  case 33: /* call_args: call_args T_COMMA expr  */
#line 142 "Parser.y"
                                 {(yyvsp[-2].exprvec)->push_back((yyvsp[0].expr));}
#line 1391 "Parser.cpp"
    break;

  case 34: /* number: T_DOUBLE  */
#line 147 "Parser.y"
                {
                    (yyval.expr) = new DoubleExprAST((yyvsp[0].double_value));
                }
#line 1399 "Parser.cpp"
    break;

  case 35: /* number: T_INTEGER  */
#line 150 "Parser.y"
                   {
       		(yyval.expr) = new IntegerExprAST((yyvsp[0].int_value));
       }
#line 1407 "Parser.cpp"
    break;

  case 36: /* block: T_L_LPAR stmts T_R_LPAR  */
#line 155 "Parser.y"
                                {(yyval.block) = (yyvsp[-1].block);}
#line 1413 "Parser.cpp"
    break;

  case 37: /* block: T_L_LPAR T_R_LPAR  */
#line 156 "Parser.y"
                            {(yyval.block) = new BlockAST();}
#line 1419 "Parser.cpp"
    break;

  case 38: /* func_decl: ident ident T_L_SPAR func_args T_R_SPAR block  */
#line 160 "Parser.y"
{
	PrototypeAST* proto = new PrototypeAST((yyvsp[-5].ident)->identifier,(yyvsp[-4].ident)->identifier,*(yyvsp[-2].varvec));
	BlockAST* body = (yyvsp[0].block);
	(yyval.stmt) = new FunctionAST(proto,body);
	printf("build function %s \n",(yyvsp[-4].ident)->identifier.c_str());
}
#line 1430 "Parser.cpp"
    break;

  case 39: /* func_args: %empty  */
#line 167 "Parser.y"
            { (yyval.varvec) = new std::vector<VariableDeclarationAST*>(); }
#line 1436 "Parser.cpp"
    break;

  case 40: /* func_args: var_decl  */
#line 168 "Parser.y"
                 { (yyval.varvec) = new std::vector<VariableDeclarationAST*>();
                   (yyval.varvec)->push_back((yyvsp[0].vdeclar)); }
#line 1443 "Parser.cpp"
    break;

  case 41: /* func_args: func_args T_COMMA var_decl  */
#line 170 "Parser.y"
                                   { (yyvsp[-2].varvec)->push_back((yyvsp[0].vdeclar)); }
#line 1449 "Parser.cpp"
    break;

  case 42: /* if_condition: T_IF T_L_SPAR expr T_R_SPAR block  */
#line 173 "Parser.y"
                                                 {(yyval.cond) = new ConditionAST((yyvsp[-2].expr),(yyvsp[0].block),nullptr);}
#line 1455 "Parser.cpp"
    break;

  case 43: /* if_condition: T_IF T_L_SPAR expr T_R_SPAR block T_ELSE block  */
#line 174 "Parser.y"
                                                         {(yyval.cond) = new ConditionAST((yyvsp[-4].expr),(yyvsp[-2].block),(yyvsp[0].block));}
#line 1461 "Parser.cpp"
    break;

  case 44: /* for_stmt: T_FOR T_L_SPAR for_args ';' for_args ';' for_args T_R_SPAR block  */
#line 178 "Parser.y"
        {
		(yyval.forexpr) = new ForExprAST((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].block));
	}
#line 1469 "Parser.cpp"
    break;

  case 45: /* for_args: var_decl  */
#line 182 "Parser.y"
                    {(yyval.node) = (yyvsp[0].stmt);}
#line 1475 "Parser.cpp"
    break;

  case 46: /* for_args: ident T_ASSIGN expr  */
#line 183 "Parser.y"
                              {(yyval.node) = new VariableAssignmentAST((yyvsp[-2].ident)->identifier,(yyvsp[0].expr));}
#line 1481 "Parser.cpp"
    break;

  case 47: /* for_args: expr  */
#line 184 "Parser.y"
               {(yyval.node) = (yyvsp[0].expr);}
#line 1487 "Parser.cpp"
    break;


#line 1491 "Parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= END)
        {
          /* Return failure if at end of input.  */
          if (yychar == END)
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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 187 "Parser.y"


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
