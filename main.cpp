#include <iostream>
#include "parser/Parser.hpp"
#include "parser/FileReader.h"

using namespace llvm;

int main() {
    // Open a new module.
    TheModule = llvm::make_unique<Module>("Kingtous' jit", TheContext);
//    TheModule->setDataLayout(TheJIT->getTargetMachine().createDataLayout());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
//    TheFPM->addPass(createInstructionCombiningPass());
//    // Reassociate expressions.
//    TheFPM->addPass(createReassociatePass());
//    // Eliminate Common SubExpressions.
//    TheFPM->addPass(createGVNPass());
//    // Simplify the control flow graph (deleting unreachable blocks, etc).
//    TheFPM->addPass(createCFGSimplificationPass());
    //
    FileReader reader("test.syk");
    Lexer lexer(reader);
    TheLexer = &lexer;
    int result = yyparse();

    // 生成目标代码
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto TargetTriple = sys::getDefaultTargetTriple();
    TheModule->setTargetTriple(TargetTriple);

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

    auto Filename = "output.o";
    std::error_code EC;
    raw_fd_ostream dest(Filename, EC, sys::fs::F_None);

    if (EC) {
        errs() << "Could not open file: " << EC.message();
        return 1;
    }

    legacy::PassManager pass;
    auto FileType = TargetMachine::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(*TheModule);
    dest.flush();

    outs() << "Wrote " << Filename << "\n";

    return 0;
}
