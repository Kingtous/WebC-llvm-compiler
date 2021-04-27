//
// Created by bytedance on 2021/3/12.
//

#include "Compiler.h"

#include "codegen/CodeGen.h"

#include <llvm/Support/Program.h>
#include <llvm/Support/VirtualFileSystem.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Driver/Driver.h>

#include <glibmm/iochannel.h>
#include <glibmm/spawn.h>

#include <boost/algorithm/string.hpp>

using namespace clang;

Lexer *m_lexer;

const vector <string> *getOpenSSLLibDir();

void initExternFunc() {
    // 初始化外层函数
    ExternFunctionLinker::registerHandler(new EchoFunctionHandler());
    ExternFunctionLinker::registerHandler(new SleepFunctionHandler());
    ExternFunctionLinker::registerHandler(new TimeFunctionHandler());
    ExternFunctionLinker::registerHandler(new WebFunctionHandler());
    ExternFunctionLinker::registerHandler(new KStringFunctionHandler());
    ExternFunctionLinker::registerHandler(new KsqlFunctionHandler());
}

int startAnalyze(ArgsParser *parser) {
    // 初始化外层函数
    initExternFunc();
    for (const auto &file : parser->getFiles()) {
        outs() << "正在分析 " << file << "...\n";
        auto reader = new FileReader(file);
        m_lexer = new Lexer(reader);
        TheLexer = m_lexer;
        int result = yyparse();
        if (!result) {
            auto ast = program;
            std::cout << ast->toString();
            auto val = ast->codegen();
            if (val == nullptr) {
                return RERR;
            } else {
                return ROK;
            }
        } else {
            return RERR;
        }
    }
    return RERR;
}

int genCode(const set<ArgsParser::Options> &opts, const char *outputPath) {
#ifdef SYSY_ARM
    LLVMInitializeARMTargetInfo();
    LLVMInitializeARMTarget();
    LLVMInitializeARMTargetMC();
    LLVMInitializeARMAsmParser();
    LLVMInitializeARMAsmPrinter();
#endif
#ifdef SYSY_AARCH64
    LLVMInitializeAArch64TargetInfo();
    LLVMInitializeAArch64Target();
    LLVMInitializeAArch64TargetMC();
    LLVMInitializeAArch64AsmParser();
    LLVMInitializeAArch64AsmPrinter();
#else
    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
    LLVMInitializeX86AsmParser();
    LLVMInitializeX86AsmPrinter();
#endif
    auto TargetTriple = sys::getDefaultTargetTriple();
    TheModule->setTargetTriple(TargetTriple);

    outs() << "正在生成目标代码(" << TargetTriple << ")...\n";
    std::string err;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, err);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        LogError(err.c_str());
        auto it = TargetRegistry::targets();
        errs() << "支持的targets:\n";
        auto i = it.begin();
        while (i != it.end()) {
            errs() << (*i).getName() << "\n";
            i++;
        }
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto TheTargetMachine =
            Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    const auto &Filename = (string(outputPath) + ".o");
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::F_None);

    if (EC) {
        LogError(EC.message().c_str());
        return 1;
    }

    legacy::PassManager pass;
// 临时关闭这些优化器
//    pass.add(createReassociatePass());
//    pass.add(createGVNPass());
//    pass.add(createCFGSimplificationPass());
//    pass.add(createTailCallEliminationPass());
    if (opts.find(ArgsParser::Options::PASS_TIME_ANALYSIS) != opts.end()) {
        pass.add(new TimeAnalysisPass());
    }
    auto file_type = CodeGenFileType::CGFT_ObjectFile;
    if (opts.find(ArgsParser::Options::OUTPUT_LLVMAS_FILE) != opts.end()) {
        file_type = CodeGenFileType::CGFT_AssemblyFile;
    }

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, file_type)) {
        LogError("TheTargetMachine can't emit a file of this type");
        return 1;
    }
    pass.run(*TheModule);

#ifdef CGUI
    std::string ir_buf;
    raw_string_ostream rfostream(ir_buf);
    TheModule->print(rfostream, NIL);
    rfostream.flush();
    LogInfo(ir_buf.c_str());
#else
    TheModule->print(outs(), NIL);
#endif
    dest.flush();

    LogInfo("已生成目标文件(.o/.dll)：");
    LogInfo(Filename.c_str());
    LogInfo("开始生成可执行文件");
    auto clang = llvm::sys::findProgramByName("clang++");
    if (auto ec = clang.getError()) {
        LogInfo("clang++未找到");
        LogInfo(ec.message().c_str());
        return RERR;
    }
    LogInfo(clang->c_str());
    vector<const char *> args;
    // TODO 支持多文件
    args.push_back(clang->c_str());
    args.push_back(Filename.c_str());
    // 链接openssl
    auto openssl_libs = getOpenSSLLibDir();
    if (openssl_libs == nullptr) {
        LogError("openssl库没找到，无法编译");
        return RERR;
    }
    for (const auto &item : *openssl_libs) {
        args.push_back(item.c_str());
    }
    args.push_back("-L/usr/local/lib");
    args.push_back("-lksql");
    args.push_back("-lmysqlcppconn");
    args.push_back("-lkweb");
    args.push_back("-lktime");
    args.push_back("-lkjson");
    args.push_back("-lkstring");
    // 输出文件
    args.push_back("-o");
    args.push_back(outputPath);
    IntrusiveRefCntPtr <clang::DiagnosticIDs> DiagID(new DiagnosticIDs());
    DiagnosticsEngine diag_engine(DiagID, new DiagnosticOptions());
    driver::Driver driver(args[0], sys::getDefaultTargetTriple(), diag_engine);
    auto webc_compilation = driver.BuildCompilation(args);
    // fallback
    SmallVector<std::pair<int, const driver::Command *>, 4> failingCommands;
    const clang::driver::Command *failingCommand = nullptr;
#ifdef DEBUG_FLAG
    driver.PrintActions(*webc_compilation);
#endif
    int compile_res = driver.ExecuteCompilation(*webc_compilation, failingCommands);
    if (compile_res < 0) {
        driver.generateCompilationDiagnostics(*webc_compilation, *failingCommand);
        return RERR;
    }
    LogInfo("编译完成");
    LogInfo(outputPath);
    return ROK;
}

int analysis(std::string *buf) {
    reset();
    auto reader = new StringReader(buf);
    m_lexer = new Lexer(reader);
    // 放至全局变量
    TheLexer = m_lexer;
    int res = yyparse();
    if (!res) {
        initExternFunc();
        auto v = program->codegen();
        if (v == nullptr) {
            res = 1; //正数失败
        }
    }
    delete reader;
    delete m_lexer;
    m_lexer = NIL;
    TheLexer = NIL;
    return !res ? ROK : RERR;
}

void reset() {
    Builder.reset();
    TheModule.reset();
    TheContext = std::make_unique<llvm::LLVMContext>();
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
    TheModule = std::make_unique<llvm::Module>(APPNAME, *TheContext);
    TheCodeGenContext->reset();
}

int build(std::string *buf, const char *outputPath, const std::set<ArgsParser::Options> &opts) {
    if (analysis(buf) == ROK) {
        if (genCode(opts, outputPath) == ROK) {
            return ROK;
        } else {
            return RERR;
        }
    }
    return RERR;
}

const vector <string> *getOpenSSLLibDir() {
    vector<const char *> argv;
    auto pkgconfig = sys::findProgramByName("pkg-config");
    if (auto ec = pkgconfig.getError()) {
        LogError(ec.message().c_str());
        return nullptr;
    }
    argv.push_back(pkgconfig->c_str());
    argv.push_back("openssl");
    argv.push_back("--libs");
    auto stdoutput = new string();
    auto stderroutput = new string();
    try {
        Glib::spawn_sync("", argv, Glib::SPAWN_DEFAULT, Glib::SlotSpawnChildSetup(), stdoutput, stderroutput);
        if (stderroutput->empty()) {
            boost::trim_right(*stdoutput);
            auto v = new vector<string>();
            boost::split(*v, *stdoutput, boost::is_any_of(" "), boost::token_compress_on);
            return v;
        } else {
            LogError("查找openssl库失败，请确保openssl库已加入系统环境变量");
            LogError(stderroutput->c_str());
            return nullptr;
        }
    } catch (Glib::SpawnError &err) {
        LogError(err.what().c_str());
        return nullptr;
    }
}
