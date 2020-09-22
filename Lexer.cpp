//
// Created by 金韬 on 2020/9/21.
//

#include "Lexer.h"

int Lexer::_getNextToken() {
    static int LastChar = ' ';
    while (isspace(LastChar)) {
        LastChar = getchar();
    }
    if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifierStr = char(LastChar);
        while (isalnum((LastChar = getchar())))
            identifierStr += LastChar;

        if (identifierStr == "def")
            return tok_def;
        else if (identifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }
    if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }
    if (LastChar == '#') {
        // Comment until end of line.
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return getNextToken();
    }
    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int Lexer::getNextToken() {
    return currToken = _getNextToken();
}

Lexer::Lexer() {}
