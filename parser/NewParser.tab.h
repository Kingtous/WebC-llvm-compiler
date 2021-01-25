/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_NEWPARSER_TAB_H_INCLUDED
# define YY_YY_NEWPARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_IDENTIFIER = 258,            /* T_IDENTIFIER  */
    T_INTEGER = 259,               /* T_INTEGER  */
    T_DOUBLE = 260,                /* T_DOUBLE  */
    T_ADD = 261,                   /* T_ADD  */
    T_SUB = 262,                   /* T_SUB  */
    T_MUL = 263,                   /* T_MUL  */
    T_DIV = 264,                   /* T_DIV  */
    T_MOD = 265,                   /* T_MOD  */
    T_ASSIGN = 266,                /* T_ASSIGN  */
    T_EQU = 267,                   /* T_EQU  */
    T_N_EQU = 268,                 /* T_N_EQU  */
    T_LESS = 269,                  /* T_LESS  */
    T_GREATER = 270,               /* T_GREATER  */
    T_REVERSE = 271,               /* T_REVERSE  */
    T_LESS_EQU = 272,              /* T_LESS_EQU  */
    T_GREATER_EQU = 273,           /* T_GREATER_EQU  */
    T_COMMA = 274,                 /* T_COMMA  */
    T_SEMICOLON = 275,             /* T_SEMICOLON  */
    T_L_SPAR = 276,                /* T_L_SPAR  */
    T_R_SPAR = 277,                /* T_R_SPAR  */
    T_L_MPAR = 278,                /* T_L_MPAR  */
    T_R_MPAR = 279,                /* T_R_MPAR  */
    T_L_LPAR = 280,                /* T_L_LPAR  */
    T_R_LPAR = 281,                /* T_R_LPAR  */
    T_FOR = 282,                   /* T_FOR  */
    T_WHILE = 283,                 /* T_WHILE  */
    T_BREAK = 284,                 /* T_BREAK  */
    T_CONTINUE = 285,              /* T_CONTINUE  */
    T_IF = 286,                    /* T_IF  */
    T_ELSE = 287,                  /* T_ELSE  */
    T_RETURN = 288,                /* T_RETURN  */
    T_VOID = 289,                  /* T_VOID  */
    T_INT = 290,                   /* T_INT  */
    T_OR = 291,                    /* T_OR  */
    T_AND = 292,                   /* T_AND  */
    T_MINUS = 293,                 /* T_MINUS  */
    T_POS = 294                    /* T_POS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 16 "NewParser.y"

	Node* node;
	ExpressionAST* expr;
	StatementAST* stmt;
	BlockAST* block;
	IdentifierExprAST* IdentifierExprAST;
	VariableDeclarationAST* vdeclar;
	std::vector<VariableDeclarationAST*> *varvec;
	std::vector<ExpressionAST*> *exprvec;
	std::string *string;
	double double_value;
	int int_value;
	int token;

#line 118 "NewParser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_NEWPARSER_TAB_H_INCLUDED  */
