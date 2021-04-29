//
// Created by bytedance on 2021/2/23.
//

#ifndef SYSYPLUS_COMPILER_ARGSPARSER_H
#define SYSYPLUS_COMPILER_ARGSPARSER_H

#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

class ArgsParser {
public:
    enum Options {
        OUTPUT_LLVMAS_FILE,
        OUTPUT_EXECUTABLE,
        OUTPUT_OBJ_FILE,
        PASS_TIME_ANALYSIS
    };

    static ArgsParser* inst(int args,char** argv);

    const vector<std::string> &getFiles() const;

    const set<Options> &getOpts() const;

    const string &getOutputPath() const;

private:
    ArgsParser();
    bool verify(int i, char **pString);
    std::vector<std::string> files;
    std::set<Options> opts;
    std::string output_path;
};


#endif //SYSYPLUS_COMPILER_ARGSPARSER_H
