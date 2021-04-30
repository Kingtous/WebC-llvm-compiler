//
// Created by 金韬 on 2020/9/21.
//

#include "Lexer.h"

int Lexer::_getNextToken() {
    while (isspace(last_char)) {
        NEXTCHAR;
    }
    if (last_char == ',') {
        NEXTCHAR;
        return T_COMMA;
    }
    if (last_char == ';') {
        // 分号直接return
        NEXTCHAR;
        return ';';
    }
    if (last_char == '\''){
        string str = "";
        while ((last_char = getchar()) != '\''){
            str.insert(str.end(),last_char);
        }
        // 吃掉'
        NEXTCHAR;
        yylval.string = new string(str);
        return T_STR;
    }
    // 判断是否是标识符
    if (isalpha(last_char)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        identifierStr = char(last_char);
        while (isalnum((last_char = getchar())) || last_char == '_')
            identifierStr += last_char;

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
        } else if(identifierStr == "func_ptr"){
            return T_FUNC_SIGN;
        } else if(identifierStr == "true"){
            return T_TRUE;
        } else if(identifierStr == "false"){
            return T_FALSE;
        }
//        else if (identifierStr == "module"){
//            return T_MODULE;
//        }
        yylval.string = new std::string(identifierStr);
        return T_IDENTIFIER;
    }

    // 是符号
    if (ispunct(last_char)) {
        // 特判
        if (last_char == '(') {
            NEXTCHAR;
            return T_L_SPAR;
        } else if (last_char == ')') {
            NEXTCHAR;
            return T_R_SPAR;
        } else if (last_char == '[') {
            NEXTCHAR;
            return T_L_MPAR;
        } else if (last_char == ']') {
            NEXTCHAR;
            return T_R_MPAR;
        } else if (last_char == '{') {
            NEXTCHAR;
            return T_L_LPAR;
        } else if (last_char == '}') {
            NEXTCHAR;
            return T_R_LPAR;
        } else if (last_char == '+') {
            NEXTCHAR;
            return T_ADD;
        } else if (last_char == '-' && isspace(seek())) {
            NEXTCHAR;
            return T_SUB;
        } else if (last_char == '*') {
            NEXTCHAR;
            return T_MUL;
        } else if (last_char == '/' && seek() != '/') {
            NEXTCHAR;
            return T_DIV;
        } else if (last_char == '%') {
            NEXTCHAR;
            return T_MOD;
        } else if (last_char == '=') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_EQU;
            } else {
                NEXTCHAR;
                return T_ASSIGN;
            }
        } else if (last_char == '<') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_LESS_EQU;
            } else {
                NEXTCHAR;
                return T_LESS;
            }
        } else if (last_char == '>') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_GREATER_EQU;
            } else {
                NEXTCHAR;
                return T_GREATER;
            }
        } else if (last_char == '!') {
            if (seek() == '=') {
                getchar();
                NEXTCHAR;
                return T_N_EQU;
            }
            NEXTCHAR;
            return T_REVERSE;
        }

        identifierStr = char(last_char);


        while (ispunct((seek()))) {
            NEXTCHAR;
            identifierStr += last_char;
            // 注释
            if (identifierStr == "//") {
                while (last_char != '\n' && last_char != END_OF_FILE) {
                    last_char = getchar();
                }
                NEXTCHAR;
                return _getNextToken();
            }
            else if (identifierStr == "&&") {
                NEXTCHAR;
                return T_AND;
            }
            else if (identifierStr == "||") {
                NEXTCHAR;
                return T_OR;
            }
        }

        if (identifierStr == "-") {
            // 负数
            if (isdigit(seek())) {
                NEXTCHAR;
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

    if (isdigit(last_char) || last_char == '.') {   // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += last_char;
            NEXTCHAR;
        } while (isdigit(last_char) || last_char == '.');
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
    if (last_char == '#') {
        // Comment until end of line.
        do
            NEXTCHAR;
        while (last_char != END_OF_FILE && last_char != '\n' && last_char != '\r');

        if (last_char != END_OF_FILE)
            return getNextToken();
    }
    // Check for end of file.  Don't eat the END_OF_FILE.
    if (last_char == END_OF_FILE)
        return 0;
    // Otherwise, just return the character as its ascii value.
    int ThisChar = last_char;
    NEXTCHAR;
    return ThisChar;
}

int Lexer::getNextToken() {
    currToken = _getNextToken();
#ifdef DEBUG_FLAG //如果处于DEBUG模式的话，就打印这些字符
    if (currToken == T_IDENTIFIER) {
        std::fprintf(stderr, "KEX: read identifier: %s\n", (*yylval.string).c_str());
    } else if (currToken < 258) {
        std::fprintf(stderr, "KEX: read character: %c\n", currToken);
    } else {
        std::fprintf(stderr, "KEX: read token type: %d\n", currToken);
    }
#endif   //如果不是debug模式的话，就不打印这些字符，直接返回
    return currToken;
}

int Lexer::getchar() {
    return reader->getchar();
}

int Lexer::seek() {
    return reader->seek();
}

unsigned int Lexer::getCLineNumber() {
    return reader->getLineNo() + 1;
}

unsigned int Lexer::getCCol() {
    return reader->getCCol();
}

Lexer *TheLexer = nullptr;

int yylex() {
    return TheLexer->getNextToken();
}
