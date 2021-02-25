//
// Created by 金韬 on 2020/9/21.
//

#include "Lexer.h"

int Lexer::_getNextToken() {
    static int LastChar = ' ';
    while (isspace(LastChar)) {
        NEXTCHAR;
    }
    if (LastChar == ',') {
        NEXTCHAR;
        return T_COMMA;
    }
    if (LastChar == ';') {
        // 分号直接return
        NEXTCHAR;
        return ';';
    }
    if (LastChar == '\''){
        string str = "";
        while ((LastChar = getchar()) != '\''){
            str.insert(str.end(),LastChar);
        }
        // 吃掉'
        NEXTCHAR;
        yylval.string = new string(str);
        return T_STR;
    }
    // 判断是否是标识符
    if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifierStr = char(LastChar);
        while (isalnum((LastChar = getchar())) || LastChar == '_')
            identifierStr += LastChar;

        if (identifierStr == "if") {
            return T_IF;
        } else if (identifierStr == "el") {
            return T_ELSE;
        } else if (identifierStr == "wh") {
            return T_WHILE;
        } else if (identifierStr == "cont") {
            return T_CONTINUE;
        } else if (identifierStr == "ret") {
            return T_RETURN;
        } else if (identifierStr == "lp") {
            return T_FOR;
        } else if (identifierStr == "out") {
            return T_OUT;
        } else if (identifierStr == "const"){
            return T_CONST;
        } else if (identifierStr == "nil"){
            return T_NULL;
        }
        yylval.string = new std::string(identifierStr);
        return T_IDENTIFIER;
    }

    // 是符号
    if (ispunct(LastChar)) {
        // 特判
        if (LastChar == '(') {
            NEXTCHAR;
            return T_L_SPAR;
        } else if (LastChar == ')') {
            NEXTCHAR;
            return T_R_SPAR;
        } else if (LastChar == '[') {
            NEXTCHAR;
            return T_L_MPAR;
        } else if (LastChar == ']') {
            NEXTCHAR;
            return T_R_MPAR;
        } else if (LastChar == '{') {
            NEXTCHAR;
            return T_L_LPAR;
        } else if (LastChar == '}') {
            NEXTCHAR;
            return T_R_LPAR;
        } else if (LastChar == '+') {
            NEXTCHAR;
            return T_ADD;
        } else if (LastChar == '-' && isspace(seek())) {
            NEXTCHAR;
            return T_SUB;
        } else if (LastChar == '*') {
            NEXTCHAR;
            return T_MUL;
        } else if (LastChar == '/' && seek() != '/') {
            NEXTCHAR;
            return T_DIV;
        } else if (LastChar == '%') {
            NEXTCHAR;
            return T_MOD;
        } else if (LastChar == '=') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_EQU;
            } else {
                NEXTCHAR;
                return T_ASSIGN;
            }
        } else if (LastChar == '<') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_LESS_EQU;
            } else {
                NEXTCHAR;
                return T_LESS;
            }
        } else if (LastChar == '>') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_GREATER_EQU;
            } else {
                NEXTCHAR;
                return T_GREATER;
            }
        } else if (LastChar == '!') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_N_EQU;
            }
            NEXTCHAR;
            return T_REVERSE;
        }

        identifierStr = char(LastChar);


        while (ispunct((LastChar = getchar())))
            identifierStr += LastChar;
        // 注释
        if (identifierStr == "//") {
            while ((LastChar = getchar()) != '\n' && LastChar != EOF);
            NEXTCHAR;
            return _getNextToken();
        }
        if (identifierStr == "&&") {
            return T_AND;
        }
        if (identifierStr == "||") {
            return T_OR;
        }
        if (identifierStr == "-") {
            // 负数
            if (isdigit(seek())) {
                int token = _getNextToken();
                if (token == T_INTEGER) {
                    yylval.int_value = -yylval.int_value;
                } else if (token == T_DOUBLE) {
                    yylval.double_value = -yylval.double_value;
                }
                return token;
            } else {
                return T_SUB;
            }
        }
        yylval.string = new std::string(identifierStr);
        return T_IDENTIFIER;
    }

    if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            NEXTCHAR;
        } while (isdigit(LastChar) || LastChar == '.');
        if (NumStr.find('.') == string::npos) {
            yylval.int_value = stoi(NumStr);
#ifdef DEBUG_FLAG
            std::fprintf(stderr, "KEX: read int: %d\n", yylval.int_value);
#endif
            return T_INTEGER;
        } else {
            yylval.double_value = stod(NumStr);
#ifdef DEBUG_FLAG
            std::fprintf(stderr, "KEX: read double: %f\n", yylval.double_value);
#endif
            return T_DOUBLE;
        }
    }
    if (LastChar == '#') {
        // Comment until end of line.
        do
            NEXTCHAR;
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return getNextToken();
    }
    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return 0;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    NEXTCHAR;
    return ThisChar;
}

int Lexer::getNextToken() {
    currToken = _getNextToken();
#ifdef DEBUG_FLAG
    if (currToken == T_IDENTIFIER) {
        std::fprintf(stderr, "KEX: read identifier: %s\n", (*yylval.string).c_str());
    } else if (currToken < 258) {
        std::fprintf(stderr, "KEX: read character: %c\n", currToken);
    } else {
        std::fprintf(stderr, "KEX: read token type: %d\n", currToken);
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

unsigned int Lexer::getCLineNumber() {
    return reader.getLineNo() + 1;
}

unsigned int Lexer::getCCol() {
    return reader.getCCol();
}

Lexer *TheLexer = nullptr;

int yylex() {
    return TheLexer->getNextToken();
}
