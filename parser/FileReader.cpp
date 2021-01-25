//
// Created by 金韬 on 2020/9/23.
//
#include "../ErrHelper.h"
#include "FileReader.h"
#include <fstream>
#include <sstream>

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

char FileReader::getchar() {
    if (index < content_length) {
        return content[index++];
    } else {
        return EOF;
    }
}

char FileReader::seek() {
    if (index < content_length) {
        return content[index];
    } else {
        return EOF;
    }
}


