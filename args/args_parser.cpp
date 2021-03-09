//
// Created by 金韬 on 2021/2/23.
//

#include "args_parser.h"
#include <args/all.hpp>

using namespace Args;

CmdLine *cmd;
MultiArg* input_arg;
Arg* output_arg;
Arg* is_target_arg;
Arg* time_analysis_arg;
Help* help_arg;

ArgsParser::ArgsParser() {

}

ArgsParser *ArgsParser::inst(int args, char **argv) {
    auto ap = new ArgsParser();
    if (ap->verify(args, argv)) {
        for (const auto &v : input_arg->values()) {
            ap->files.push_back(v);
        }
        ap->output_path = output_arg->value();
        if (is_target_arg->isDefined()){
            ap->opts.insert(Options::OUTPUT_LLVMAS_FILE);
        } else {
            ap->opts.insert(Options::OUTPUT_EXECUTABLE);
        }
        // 是否加入时间分析
        if (time_analysis_arg->isDefined()){
            ap->opts.insert(Options::PASS_TIME_ANALYSIS);
        }
        return ap;
    } else {
        return nullptr;
    }
}

bool ArgsParser::verify(int args, char **argv) {
    cmd = new CmdLine(args, argv);
    //
    input_arg = new MultiArg('i',"input",true,true);
    input_arg->setDescription(String("输入源文件"));
    input_arg->setLongDescription(String("输入源文件（支持多文件）"));
    cmd->addArg(input_arg);

    output_arg = new Arg('o',"output",true,true);
    input_arg->setDescription(String("输出文件"));
    input_arg->setLongDescription(String("输出文件"));
    cmd->addArg(output_arg);

    is_target_arg = new Arg('s',"as",false,false);
    is_target_arg->setDescription("是否生成可读汇编文件");
    is_target_arg->setLongDescription("source code -> llvm ir -> assembly file(可视化) -> object file");
    cmd->addArg(is_target_arg);

    time_analysis_arg = new Arg('t',"time_analysis",false,false);
    time_analysis_arg->setDescription("是否在每个函数内加入时间分析");
    time_analysis_arg->setLongDescription("add time analysis into every function");
    cmd->addArg(time_analysis_arg);

    help_arg = new Help();
    help_arg->setExecutable(argv[0]);
    help_arg->setAppDescription("SysyPlus Compiler By Kingtous.");
    cmd->addArg(help_arg);
    try {
        cmd->parse();
        auto re = cmd != nullptr &&
                  input_arg->isDefined() && output_arg->isDefined();
        if (!re) {
            fprintf(stderr, "输入输出未定义");
            return false;
        } else {
            return true;
        }
    } catch (const HelpHasBeenPrintedException &) {
        return false;
    }
    catch (const BaseException &x) {
        outStream() << x.desc() << "\n使用-h/--help查看帮助\n";
        return false;
    }
}

const vector<std::string> &ArgsParser::getFiles() const {
    return files;
}

const set<ArgsParser::Options> &ArgsParser::getOpts() const {
    return opts;
}

const string &ArgsParser::getOutputPath() const {
    return output_path;
}
