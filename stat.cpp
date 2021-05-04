//
// Created by kingtous on 2021/5/3.
//

#include "stat.h"

#include <iostream>

namespace webc {
    namespace lex {
        int token_num = 0;
        int line_num = 0;
    }
    namespace parser {
        int condition_num = 0;
        int loop_num = 0;
        int func_num = 0;
        int func_params_num = 0;
        int func_pointers_num = 0;
    }
    namespace ast {
        int layers = 0;
        int nodes_num = 0;
    }

    void outputStats() {
        printf("WebC Code Stats:\nlexer:tokennum:%d,linenum:%d\n"
               "parser:condition:%d,loop:%d,funcs:%d,fparam:%d,fpoint:%d\n"
               "ast:layers:%d,node_num:%d\n", lex::token_num, lex::line_num, parser::condition_num, parser::loop_num,
               parser::func_num, parser::func_params_num, parser::func_pointers_num, ast::layers, ast::nodes_num);
    }
}