%code top {
  #include <stdio.h>
}

%code requires {
#include "Lexer.h"
 #include "ast/NodeAST.hpp"
// 程序分析入口点
extern BlockAST* program;
using std::vector;
}

%{
#include "Lexer.h"
extern char *yytext;
void yyerror(const char *s)
{ fprintf(stderr,"%s on line %d, Col %d\n",s,TheLexer->getCLineNumber(),TheLexer->getCCol()); }
%}

// 声明变量可能有的类型
%union{
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
	std::vector<VariableDeclarationAST*> *varvec;
	std::vector<ExpressionAST*> *exprvec;
	std::vector<ExpressionAST*> *aivec; // array index vector
	vector<NodeAST*>* arrayvalvec;
	std::string *string;
	double double_value;
	int int_value;
	int token;
}
/// 终结符
// 设置结点对应的类型
%token <string> T_IDENTIFIER
%token <int_value> T_INTEGER
%token <double_value> T_DOUBLE
// 计算符号
%token <token> T_ADD T_SUB T_MUL T_DIV T_MOD T_ASSIGN
// 比较符
%token <token> T_EQU T_N_EQU T_LESS T_GREATER T_REVERSE T_LESS_EQU T_GREATER_EQU
// , ; () [] {}
%token <token> T_COMMA T_SEMICOLON T_L_SPAR T_R_SPAR T_L_MPAR T_R_MPAR T_L_LPAR T_R_LPAR T_CONST
// ' "
%token <token> T_STR T_NULL
// 循环
%token <token> T_FOR T_WHILE T_OUT T_CONTINUE
%token <token> T_IF T_ELSE
%token <token> T_RETURN
%token <token> T_VOID T_INT
%token END 0 "END OF FILE BY SYSYPLUS COMPILER BY KINGTOUS"
/// 非终结符
// type 类型 为此类型的名称
%type <block> program block stmts
%type <ident> ident
%type <identarr> ident_arr
%type <expr> number expr str
%type <cond> if_condition
%type <forexpr> for_stmt
%type <whilestmt> while_stmt
%type <node> for_args
%type <stmt> stmt func_decl
%type <vdeclar> var_decl
%type <varvec> func_args
%type <exprvec> call_args
%type <arrayvalvec> array_init_val array_init_list
%type <aivec> array_index "array index vector"
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

program : stmts{
			program = $1;
			#ifdef DEBUG_FLAG
			fprintf(stdout,"parse success\n");
			#endif
		}
	;

stmts : stmt {$$ = new BlockAST(); $$->statements.push_back($<stmt>1);}
	| stmts stmt {$1->statements.push_back($<stmt>2);}
	;

stmt : var_decl ';' {
#ifdef DEBUG_FLAG
fprintf(stderr,"build var decl stmt\n");
#endif
}
	| T_CONST var_decl ';' {$2->setIsConst(true);$$=$2;}
	| func_decl {$$ = $1;}
	| if_condition {$$ = $1;}
	| while_stmt {$$ = $1;}
	| ident T_ASSIGN expr ';' {$$ = new VariableAssignmentAST($1->identifier,$3);}
	| ident_arr T_ASSIGN expr ';' {$$ = new VariableArrAssignmentAST($1,$3);}
	| T_RETURN ';'{$$ = new ReturnStmtAST();}
	| T_RETURN expr ';'{$$ = new ReturnStmtAST($2);}
	| for_stmt {$$ = $1;}
	| T_OUT ';' {$$ = new OutStmtAST();}
	| T_CONTINUE ';' {$$ = new ContinueStmtAST();}
	| block {$$ = $1;}
	| ';' {}
	| expr ';' {$$ = new ExpressionStatementAST($1);}
	;

ident : T_IDENTIFIER {
 $$ = new IdentifierExprAST(*$1);}
      ;
// a[xx][xx]...
ident_arr : ident array_index { $$ = new IdentifierArrExprAST($1->identifier,$2);}

// int a
// int a = 123
var_decl : ident ident {
		$$ = new VariableDeclarationAST(
		$1->identifier,
		$2
		);
		}
	| ident ident T_ASSIGN expr {$$ = new VariableDeclarationAST($1->identifier,$2,$4);}
	| ident ident_arr {
          		$$ = new VariableArrDeclarationAST(
          		$1->identifier,
          		$2
          		);
          		}
         | ident ident_arr T_ASSIGN array_init_val {$$ = new VariableArrDeclarationAST($1->identifier,$2,$4);}
	;


// func() func(a,b) func(a)
// a;
// a<b
expr : ident T_L_SPAR call_args T_R_SPAR {$$ = new CallExprAST($1->identifier,*$3); }
	| ident {$<ident>$ = $1;}
	| ident_arr {$<ident>$ = $1;}
	| T_SUB ident {$$ = new BinaryExprAST(BinaryType::sub,new IntegerExprAST(0),$2);}
	| number
	| T_L_SPAR expr T_R_SPAR {$$ = $2;}
	| expr T_ADD expr {$$ = new BinaryExprAST(BinaryType::add,$1,$3);}
	| expr T_SUB expr {$$ = new BinaryExprAST(BinaryType::sub,$1,$3);}
	| expr T_MUL expr {$$ = new BinaryExprAST(BinaryType::mul,$1,$3);}
	| expr T_DIV expr {$$ = new BinaryExprAST(BinaryType::divi,$1,$3);}
	| expr T_MOD expr {$$ = new BinaryExprAST(BinaryType::mod,$1,$3);}
	| expr T_LESS expr {$$ = new BinaryExprAST(BinaryType::less,$1,$3);}
	| expr T_GREATER expr {$$ = new BinaryExprAST(BinaryType::greater,$1,$3);}
	| expr T_LESS_EQU expr {$$ = new BinaryExprAST(BinaryType::less_equ,$1,$3);}
	| expr T_GREATER_EQU expr {$$ = new BinaryExprAST(BinaryType::greater_equ,$1,$3);}
	| expr T_EQU expr {$$ = new BinaryExprAST(BinaryType::equ,$1,$3);}
	| expr T_N_EQU expr {$$ = new BinaryExprAST(BinaryType::n_equ,$1,$3);}
	| expr T_AND expr {$$ = new BinaryExprAST(BinaryType::AND,$1,$3);}
	| expr T_OR expr {$$ = new BinaryExprAST(BinaryType::OR,$1,$3);}
	| str {$$ = $1;}
	| T_NULL {$$ = new NullExprAST();}
	;
//aivec
array_index : T_L_MPAR T_R_MPAR {$$ = new vector<ExpressionAST*>(); $$->push_back(NIL);}
	| T_L_MPAR expr T_R_MPAR {$$ = new vector<ExpressionAST*>(); $$->push_back($2);}
	| T_L_MPAR T_R_MPAR array_index{$3->insert($3->begin(),NIL); $$ = $3;}
	| T_L_MPAR expr T_R_MPAR array_index {$4->insert($4->begin(),$2); $$ = $4;}

call_args : %empty  {$$ = new std::vector<ExpressionAST*>();}
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
	// printf("build function %s \n",$2->identifier.c_str());
}

func_args : %empty { $$ = new std::vector<VariableDeclarationAST*>(); }
      | var_decl { $$ = new std::vector<VariableDeclarationAST*>();
                   $$->push_back($<vdeclar>1); } //$1为stmt类型，但是我们要取子类vdeclar类型
      | func_args T_COMMA var_decl { $1->push_back($<vdeclar>3); }
      ;

if_condition : T_IF T_L_SPAR expr T_R_SPAR block {$$ = new ConditionAST($3,$5,nullptr);}
	| T_IF T_L_SPAR expr T_R_SPAR block T_ELSE block {$$ = new ConditionAST($3,$5,$7);}
	;

for_stmt : T_FOR T_L_SPAR for_args ';' for_args ';' for_args T_R_SPAR block
	{
		$$ = new ForExprAST($3,$5,$7,$9);
	}

for_args : var_decl {$$ = $1;}
	| ident T_ASSIGN expr {$$ = new VariableAssignmentAST($1->identifier,$3);}
	| expr {$$ = $1;}
	;

while_stmt : T_WHILE T_L_SPAR expr T_R_SPAR block {$$ = new WhileStmtAST($3,$5);}

// {{1,2,3,4},{1,123,23,3}};
array_init_val :  expr {$$ = new vector<NodeAST*>(); $$->push_back($1);}
	| T_L_LPAR array_init_list T_R_LPAR {$$ = $2;}
	| T_L_LPAR T_R_LPAR {$$ = new vector<NodeAST*>();}
	;

array_init_list : array_init_list T_COMMA array_init_val {$$ = $1; $1->insert($1->end(),$3->begin(),$3->end());}
	| array_init_val {$$ = $1;}

str : T_STR {$$ = new StringExprAST($<string>$);}

module_stmt : T_MODULE
%%

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
