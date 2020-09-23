//
// Created by 金韬 on 2020/9/23.
//

#ifndef LLVM_KALEIDOSCOPE_FILEREADER_H
#define LLVM_KALEIDOSCOPE_FILEREADER_H

#include <string>

#include "file_reader_wrapper.h"

class FileReader : public FileReaderWrapper {

    std::string content;
    unsigned int index = 0;

public:
    explicit FileReader(const std::string &path);

    char getchar() override;

    char seek() override;

    unsigned long content_length;
};


#endif //LLVM_KALEIDOSCOPE_FILEREADER_H
