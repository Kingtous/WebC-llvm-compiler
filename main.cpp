#include <iostream>
#include "args_parser.h"
#include "parser/Parser.hpp"
#include "parser/FileReader.h"
#include "Global.h"
#include "ui/widgets.h"
#include "ErrHelper.h"
#include "pass/TimeAnalysisPass.h"

using namespace llvm;
Lexer *m_lexer;

#ifdef CGUI
#include <gtkmm.h>
static void activate(GtkApplication *app, gpointer data);
#endif

int startAnalyze(ArgsParser *parser);

int genCode(ArgsParser *parser);

int main(int argc, char **argv) {
#ifdef CGUI
    auto app = Gtk::Application::create(argc, argv, APPNAME, Gio::APPLICATION_FLAGS_NONE);
    auto builder = Gtk::Builder::create_from_file("../ui/ui/main.glade");
    Gtk::Window *window = NIL;
    builder->get_widget("main_window", window);
    if (window == NIL) {
        LogError("main_window 未找到");
        return -1;
    }
    window->set_size_request(1200,800);
    return app->run(*window);
#else
    // Open a new module.
    auto args = ArgsParser::inst(argc,argv);
    if (args == NIL){
        return INVALID;
    }
    if (startAnalyze(args) == OK){
        if (genCode(args) == OK){
            return OK;
        } else {
            errs() << "生成代码失败";
        }
    } else {
        errs() << "分析失败";
    }
#endif
    return OK;
}

int genCode(ArgsParser* parser) {
    auto opts = parser->getOpts();
    // 生成目标代码，目前只初始化x86
    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
    LLVMInitializeX86AsmParser();
    LLVMInitializeX86AsmPrinter();

    auto TargetTriple = sys::getDefaultTargetTriple();
    TheModule->setTargetTriple(TargetTriple);
    outs() << "正在生成目标代码(" << TargetTriple << ")...\n";
    std::string Error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        errs() << Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto TheTargetMachine =
            Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    const auto &Filename = parser->getOutputPath();
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::F_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }

    legacy::PassManager pass;
    pass.add(createReassociatePass());
    pass.add(createGVNPass());
    pass.add(createCFGSimplificationPass());
    pass.add(createTailCallEliminationPass());
    if (opts.find(ArgsParser::Options::PASS_TIME_ANALYSIS) != opts.end()){
        pass.add(new TimeAnalysisPass());
    }
    auto file_type = CodeGenFileType::CGFT_ObjectFile;
    if (opts.find(ArgsParser::Options::OUTPUT_LLVMAS_FILE) != opts.end()) {
        file_type = CodeGenFileType::CGFT_AssemblyFile;
    }

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, file_type)) {
        errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }
    pass.run(*TheModule);
    TheModule->print(outs(),NIL);
    dest.flush();

    outs() << "已生成目标文件(.o/.dll)： " << Filename << "\n";
    return OK;
}

int startAnalyze(ArgsParser* parser) {
    // 初始化外层函数
    ExternFunctionLinker::registerHandler(new EchoFunctionHandler());
    ExternFunctionLinker::registerHandler(new SleepFunctionHandler());
    ExternFunctionLinker::registerHandler(new TimeFunctionHandler());
    ExternFunctionLinker::registerHandler(new WebFunctionHandler());
    for (const auto& file : parser->getFiles()){
        outs() << "正在分析 " << file << "...\n";
        FileReader reader(file);
        m_lexer = new Lexer(reader);
        TheLexer = m_lexer;
        int result = yyparse();
        if (!result) {
            auto ast = program;
            //std::cout << ast->toString();
            auto val = ast->codegen();
            if (val == nullptr) {
                return ERR;
            } else {
                return OK;
            }
        } else {
            return ERR;
        }
    }
    return ERR;
}