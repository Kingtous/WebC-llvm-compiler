%code top {
  #define _GNU_SOURCE
  #include <stdio.h>
}

%code requires {
#include "Lexer.h"
  #include "ast/NodeAST.h"
// 程序分析入口点
static BlockAST* program;
using std::vector;
  extern FILE * yyin;
  extern FILE * yyout;
}

%{
void yyerror(const char *s) { printf("ERROR: %sn", s); }
%}

// 声明变量可能有的类型
%union{
	NodeAST* node;
	ExpressionAST* expr;
	StatementAST* stmt;
	BlockAST* block;
	IdentifierExprAST* ident;
	VariableDeclarationAST* vdeclar;
	std::vector<VariableDeclarationAST*> *varvec;
	std::vector<ExpressionAST*> *exprvec;
	std::string *string;
	double double_value;
	int int_value;
	int token;
}
/// 终结符
// 设置结点对应的类型
%token <ident> T_IDENTIFIER
%token <int_value> T_INTEGER
%token <double_value> T_DOUBLE
// 计算符号
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_ASSIGN
// 比较符
%token <token> T_EQU T_N_EQU T_LESS T_GREATER T_REVERSE T_LESS_EQU T_GREATER_EQU
// , ; () [] {}
%token <token> T_COMMA T_SEMICOLON T_L_SPAR T_R_SPAR T_L_MPAR T_R_MPAR T_L_LPAR T_R_LPAR
// 循环
%token <ident> T_FOR T_WHILE T_BREAK T_CONTINUE
%token <token> T_IF T_ELSE
%token <token> T_RETURN
%token <token> T_VOID T_INT
%token <token> T_EOF
/// 非终结符
// type 类型 为此类型的名称
%type <block> program block stmts
%type <ident> ident
%type <expr> number expr
%type <stmt> stmt var_decl func_decl
%type <varvec> func_args
%type <exprvec> call_args
// 标识 or 优先级 > and
%left T_OR
%left T_AND
%left T_ASSIGN T_EQU T_N_EQU T_LESS T_GREATER T_LESS_EQU T_GREATER_EQU
%left T_ADD T_SUB
%left T_MUL T_DIV T_MOD

// 标识无关token，在处理优先级时忽略
%nonassoc T_L_MPAR
%nonassoc T_R_SPAR
%nonassoc T_MINUS T_POS T_REVERSE
%nonassoc T_ELSE
%nonassoc T_VOID T_INT
// 开始为program
%start program
%%

program : stmts {
			program = $1;
		}
	;

stmts : stmt {$$ = new BlockAST(); $$->statements.push_back($<stmt>1);}
	| stmts stmt {$1->statements.push_back($<stmt>2);}
	;

stmt : var_decl
	| func_decl
	;

ident : T_IDENTIFIER { $$ = new IdentifierExprAST(*$1); delete $1;}
      ;

// int a
// int a = 123
var_decl : ident ident {
		$$ = new VariableDeclarationAST(
		$1->identifier,
		$2->identifier
		);
		}
	| ident ident T_ASSIGN expr {$$ = new VariableDeclarationAST($1->identifier,$2->identifier,$4);}
	;


// a=b;
// func() func(a,b) func(a)
// a;
// a<b
expr : ident T_ASSIGN expr {$$ = new VariableAssignmentAST($1->identifier,$3);}
	| ident T_L_SPAR call_args T_R_SPAR {$$ = new CallExprAST($1->identifier,*$3); }
	| ident {$<ident>$ = $1;}
	| number
	| T_L_SPAR expr T_R_SPAR {$$ = $2;}
	| expr T_ADD expr {$$ = new BinaryExprAST($2,$1,$3);}
	| expr T_SUB expr {$$ = new BinaryExprAST($2,$1,$3);}
	| expr T_MUL expr {$$ = new BinaryExprAST($2,$1,$3);}
	| expr T_DIV expr {$$ = new BinaryExprAST($2,$1,$3);}
	| expr T_MOD expr {$$ = new BinaryExprAST($2,$1,$3);}
	;

call_args :  {$$ = new std::vector<ExpressionAST*>();}
	| expr {$$ = new std::vector<ExpressionAST*>();$$->push_back($1);}
	| call_args T_COMMA expr {$1->push_back($3);}
	;


number : T_DOUBLE
                {
                    $$ = new DoubleExprAST($1);
                }
       | T_INTEGER {
       		$$ = new IntegerExprAST($1);
       }
       ;

block : T_L_LPAR stmts T_R_LPAR {$$ = $2;}
	| T_L_LPAR T_R_LPAR {$$ = new BlockAST();}

// int func_name(expr1,...){block}
func_decl : ident ident T_L_SPAR func_args T_R_SPAR block
{
	PrototypeAST* proto = new PrototypeAST($1->identifier,$2->identifier,*$4);
	BlockAST* body = $6;
	$$ = new FunctionAST(proto,body);
}

func_args : { $$ = new std::vector<VariableDeclarationAST*>(); }
      | var_decl { $$ = new std::vector<VariableDeclarationAST*>();
                   $$->push_back($<vdeclar>1); } //$1为stmt类型，但是我们要取子类vdeclar类型
      | func_args T_COMMA var_decl { $1->push_back($<vdeclar>3); }
      ;

%%

BlockAST* run_parser(std::string path) {
    FILE* yyin = fopen(path.c_str(), "r+");
    program = nullptr;
    int ret = yyparse();
    if (ret != 0)
        program = nullptr;
    fclose(yyin);
    return program;
}
