//
// Created by 金韬 on 2020/9/21.
//

#ifndef LLVM_KALEIDOSCOPE_FILE_READER_WRAPPER_H
#define LLVM_KALEIDOSCOPE_FILE_READER_WRAPPER_H

class FileReaderWrapper{
public:
    virtual char readNext(){ return 0; };

};

#endif //LLVM_KALEIDOSCOPE_FILE_READER_WRAPPER_H
