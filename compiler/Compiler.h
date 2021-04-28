//
// Created by bytedance on 2021/3/12.
//

#ifndef SYSYPLUS_COMPILER_COMPILER_H
#define SYSYPLUS_COMPILER_COMPILER_H

#include <set>

#include <giomm.h>

#include "args_parser.h"
#include "Global.h"
#include "ErrHelper.h"
#include "pass/TimeAnalysisPass.h"
#include "pass/RecursiveToLoopPass.h"
#include "parser/FileReader.h"
#include "parser/StringReader.h"
#include "Global.h"
#include "parser/Parser.hpp"

extern Lexer *m_lexer;

/// 终端输入

int genCode(const set<ArgsParser::Options>& opts, const char *outputPath);

int startAnalyze(ArgsParser* parser);

/// 可视化调用

int analysis(std::string* buf);

int build(std::string *buf, const char* outputPath,const std::set<ArgsParser::Options>& opts);

/**
 *  重置编译器
 */
void reset();

#endif //SYSYPLUS_COMPILER_COMPILER_H
