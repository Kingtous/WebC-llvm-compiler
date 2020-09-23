#include <iostream>
#include "Parser.h"
#include "FileReader.h"

int main() {
    FileReader reader("/Users/kingtous/CLionProjects/llvm-kaleidoscope/test.kl");
    Lexer lexer(reader);
    Parser parser(lexer);
    parser.test();
    return 0;
}
