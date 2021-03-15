#include <iostream>
#include "args_parser.h"
#include "parser/Parser.hpp"
#include "Global.h"
#include "ui/widgets.h"
#include "compiler/Compiler.h"

using namespace llvm;

#ifdef CGUI

#include "ui/widgets.h"

static void activate(GtkApplication *app, gpointer data);

#endif

int main(int argc, char **argv) {
#ifdef CGUI
    return initWindow(argc, argv, "../ui/ui/main.glade", APPNAME);
#else
    // Open a new module.
    auto args = ArgsParser::inst(argc, argv);
    if (args == NIL) {
        return RINVALID;
    }
    if (startAnalyze(args) == ROK) {
        if (genCode(args->getOpts(), args->getOutputPath().c_str()) == ROK) {
            return ROK;
        } else {
            errs() << "生成代码失败";
        }
    } else {
        errs() << "分析失败";
    }
#endif
    return ROK;
}