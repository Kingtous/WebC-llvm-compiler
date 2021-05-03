//
// Created by 金韬 on 2020/9/23.
//
#include "../ErrHelper.h"
#include "FileReader.h"
#include <fstream>
#include <sstream>

#include "stat.h"

FileReader::FileReader(const std::string &path) {
    std::ifstream ifs(path);
    if (ifs.is_open()) {
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        ifs.close();
        content = std::string(buffer.str());
        content_length = content.size();
    } else {
        fprintf(stderr, "err open files");
        exit(-1);
    }
}

int FileReader::getchar() {
    if (index < content_length) {
        char ch = content[index++];
        if (ch == '\n') {
            lineno++;
            colno = 0;
            colno++;
            webc::lex::line_num++;
        }
        return ch;
    } else {
        return END_OF_FILE;
    }
}

int FileReader::seek() {
    if (index < content_length) {
        return content[index];
    } else {
        return EOF;
    }
}


unsigned int FileReader::getLineNo() {
    return lineno;
}

unsigned int FileReader::getCCol() {
    return colno;
}

