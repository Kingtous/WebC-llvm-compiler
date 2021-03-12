//
// Created by bytedance on 2021/3/12.
//

#ifndef SYSYPLUS_COMPILER_COMPILER_H
#define SYSYPLUS_COMPILER_COMPILER_H

#include "args_parser.h"
#include "Global.h"
#include "ErrHelper.h"
#include "pass/TimeAnalysisPass.h"
#include "parser/FileReader.h"
#include "Global.h"
#include "parser/Parser.hpp"

extern Lexer *m_lexer;

int genCode(ArgsParser* parser);

int startAnalyze(ArgsParser* parser);


#endif //SYSYPLUS_COMPILER_COMPILER_H
