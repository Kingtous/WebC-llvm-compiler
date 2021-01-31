//
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_IFILEREADER_H
#define LLVM_KALEIDOSCOPE_IFILEREADER_H

class IFileReader {
public:
    virtual char getchar() = 0;

    virtual char seek() = 0;

};


#endif //LLVM_KALEIDOSCOPE_IFILEREADER_H
