/* A Bison parser, made by GNU Bison 3.5.1.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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

#line 56 "Parser.hpp"

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

#line 178 "Parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
