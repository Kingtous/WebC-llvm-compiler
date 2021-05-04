//
// Created by kingtous on 2021/5/3.
//

#ifndef WEBC_COMPILER_STAT_H
#define WEBC_COMPILER_STAT_H

#define UPDATE_MAX_LAYERS(x) webc::ast::layers = max(x,webc::ast::layers)

namespace webc {
    namespace lex {
        extern int token_num;
        extern int line_num;
    }
    namespace parser {
        extern int condition_num;
        extern int loop_num;
        extern int func_num;
        extern int func_params_num;
        extern int func_pointers_num;
    }
    namespace ast {
        extern int layers;
        extern int nodes_num;
    }

    /**
     * 输出统计结果
     */
    void outputStats();
}

#endif //WEBC_COMPILER_STAT_H
