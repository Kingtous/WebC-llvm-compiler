//
// Created by 金韬 on 2020/9/21.
//

#include "Lexer.h"

int Lexer::_getNextToken() {
    static int LastChar = ' ';
    while (isspace(LastChar)) {
        LastChar = getchar();
    }
    if (LastChar == ',') {
        return T_COMMA;
    }
    if (LastChar == ';') {
        // 分号直接return
        LastChar = getchar();
        return ';';
    }
    // 判断是否是标识符
    if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifierStr = char(LastChar);
        while (isalnum((LastChar = getchar())))
            identifierStr += LastChar;

        if (identifierStr == "if") {
            return T_IF;
        } else if (identifierStr == "else") {
            return T_ELSE;
        }
        else if (identifierStr == "while"){
            return T_WHILE;
        } else if (identifierStr == "break") {
            return T_BREAK;
        } else if (identifierStr == "continue") {
            return T_CONTINUE;
        } else if (identifierStr == "return") {
            return T_RETURN;
        }
        yylval.string = new std::string(identifierStr);
        return T_IDENTIFIER;
    }

    // 是符号
    if (ispunct(LastChar)) {
        // 特判
        if (LastChar == '(') {
            LastChar = getchar();
            return T_L_SPAR;
        } else if (LastChar == ')') {
            LastChar = getchar();
            return T_R_SPAR;
        } else if (LastChar == '[') {
            LastChar = getchar();
            return T_L_MPAR;
        } else if (LastChar == ']') {
            LastChar = getchar();
            return T_R_MPAR;
        } else if (LastChar == '{') {
            LastChar = getchar();
            return T_L_LPAR;
        } else if (LastChar == '}') {
            LastChar = getchar();
            return T_R_LPAR;
        }
        identifierStr = char(LastChar);


        while (ispunct((LastChar = getchar())))
            identifierStr += LastChar;
        if (identifierStr == "==") {
            return T_EQU;
        } else if (identifierStr == "=") {
            return T_ASSIGN;
        }
        yylval.string = new std::string(identifierStr);
        return T_IDENTIFIER;
    }

    if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');
        // TODO 教程此处有问题，此处我认为需要更新LastChar：否则def test(a) a+1;报错
        yylval.double_value = strtod(NumStr.c_str(), nullptr);
#ifdef DEBUG_FLAG
        std::fprintf(stderr,"read double: %f\n", yylval.double_value);
#endif
        return T_DOUBLE;
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
        return 0;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int Lexer::getNextToken() {
    currToken = _getNextToken();
#ifdef DEBUG_FLAG
    if (currToken == T_IDENTIFIER){
        std::fprintf(stderr,"KEX: read identifier: %s\n", (*yylval.string).c_str());
    } else if (currToken < 258){
        std::fprintf(stderr,"KEX: read character: %c\n", currToken);
    } else {
        std::fprintf(stderr,"KEX: read token type: %d\n", currToken);
    }
#endif
    return currToken;
}

char Lexer::getchar() {
    return reader.getchar();
}

char Lexer::seek() {
    return reader.seek();
}

Lexer* TheLexer = nullptr;

int yylex() {
    return TheLexer->getNextToken();
}
