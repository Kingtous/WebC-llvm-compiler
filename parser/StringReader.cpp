//
// Created by bytedance on 2021/3/15.
//

#include "StringReader.h"

char StringReader::getchar() {
    if (cindex >= str->length()){
        return EOF;
    }
    char ch = str->at(cindex++);
    if (ch == '\n'){
        lineno++;
        colno = 0;
    } else {
        colno++;
    }
    return ch;
}

char StringReader::seek() {
    if (cindex >= str->length()){
        return EOF;
    }
    return str->at(cindex);
}

unsigned int StringReader::getLineNo() {
    return lineno;
}

unsigned int StringReader::getCCol() {
    return colno;
}

StringReader::StringReader(std::string *str) : str(str) {}
