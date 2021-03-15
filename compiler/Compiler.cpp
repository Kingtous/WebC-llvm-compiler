//
// Created by bytedance on 2021/3/12.
//

#include "Compiler.h"

Lexer *m_lexer;

int genCode(ArgsParser *parser)
{
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
    if (!Target)
    {
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

    if (EC)
    {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }

    legacy::PassManager pass;
    pass.add(createReassociatePass());
    pass.add(createGVNPass());
    pass.add(createCFGSimplificationPass());
    pass.add(createTailCallEliminationPass());
    if (opts.find(ArgsParser::Options::PASS_TIME_ANALYSIS) != opts.end())
    {
        pass.add(new TimeAnalysisPass());
    }
    auto file_type = CodeGenFileType::CGFT_ObjectFile;
    if (opts.find(ArgsParser::Options::OUTPUT_LLVMAS_FILE) != opts.end())
    {
        file_type = CodeGenFileType::CGFT_AssemblyFile;
    }

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, file_type))
    {
        errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }
    pass.run(*TheModule);
    TheModule->print(outs(), NIL);
    dest.flush();

    outs() << "已生成目标文件(.o/.dll)： " << Filename << "\n";
    return ROK;
}

int startAnalyze(ArgsParser *parser)
{
    // 初始化外层函数
    ExternFunctionLinker::registerHandler(new EchoFunctionHandler());
    ExternFunctionLinker::registerHandler(new SleepFunctionHandler());
    ExternFunctionLinker::registerHandler(new TimeFunctionHandler());
    ExternFunctionLinker::registerHandler(new WebFunctionHandler());
    for (const auto &file : parser->getFiles())
    {
        outs() << "正在分析 " << file << "...\n";
        auto reader = new FileReader(file);
        m_lexer = new Lexer(reader);
        TheLexer = m_lexer;
        int result = yyparse();
        if (!result)
        {
            auto ast = program;
            //std::cout << ast->toString();
            auto val = ast->codegen();
            if (val == nullptr)
            {
                return RERR;
            }
            else
            {
                return ROK;
            }
        }
        else
        {
            return RERR;
        }
    }
    return RERR;
}

int analysis(std::string *buf)
{
    reset();
    auto reader = new StringReader(buf);
    m_lexer = new Lexer(reader);
    // 放至全局变量
    TheLexer = m_lexer;
    int res = yyparse();
    if (!res)
    {
        auto v = program->codegen();
    }
    delete reader;
    delete m_lexer;
    delete buf;
    m_lexer = NIL;
    TheLexer = NIL;
    return !res ? ROK : RERR;
}

void reset()
{
    // TODO 可能有内存泄漏
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
    // 猜测：TheContext可析构
//    TheModule.();
    TheModule = std::make_unique<llvm::Module>("Kingtous JIT",*TheContext);
    TheContext = std::make_unique<llvm::LLVMContext>();
}