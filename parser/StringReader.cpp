//
// Created by bytedance on 2021/3/15.
//

#include "StringReader.h"

int StringReader::getchar() {
    if (cindex >= str->length()) {
        return END_OF_FILE;
    }
    char ch = str->at(cindex++);
    if (ch == '\n') {
        lineno++;
        colno = 0;
    } else {
        colno++;
    }
    return ch;
}

int StringReader::seek() {
    if (cindex >= str->length()) {
        return END_OF_FILE;
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
