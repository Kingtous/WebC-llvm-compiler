//
// Created by 金韬 on 2020/9/23.
//

#ifndef LLVM_KALEIDOSCOPE_FILEREADER_H
#define LLVM_KALEIDOSCOPE_FILEREADER_H

#include <string>

#include "IFileReader.h"

class FileReader : public IFileReader {

    std::string content;
    unsigned int index = 0;
    unsigned int lineno = 0;
    unsigned int colno = 0;

public:
    explicit FileReader(const std::string &path);

    int getchar() override;

    int seek() override;

    unsigned long content_length;

    unsigned int getLineNo() override;

    unsigned int getCCol() override;
};


#endif //LLVM_KALEIDOSCOPE_FILEREADER_H
