//
// Created by bytedance on 2021/3/12.
//

#include "Compiler.h"

#include "codegen/CodeGen.h"

Lexer *m_lexer;

void initExternFunc() {
    // 初始化外层函数
    ExternFunctionLinker::registerHandler(new EchoFunctionHandler());
    ExternFunctionLinker::registerHandler(new SleepFunctionHandler());
    ExternFunctionLinker::registerHandler(new TimeFunctionHandler());
    ExternFunctionLinker::registerHandler(new WebFunctionHandler());
    ExternFunctionLinker::registerHandler(new KStringFunctionHandler());
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
    // 生成目标代码，目前只初始化x86
    LLVMInitializeX86TargetInfo();
    LLVMInitializeX86Target();
    LLVMInitializeX86TargetMC();
    LLVMInitializeX86AsmParser();
    LLVMInitializeX86AsmPrinter();

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
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    auto TheTargetMachine =
            Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(TheTargetMachine->createDataLayout());

    const auto &Filename = outputPath;
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
    LogInfo(Filename);
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
