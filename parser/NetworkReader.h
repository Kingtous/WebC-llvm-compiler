//
// Created by kingtous on 2021/5/17.
//

#ifndef WEBC_COMPILER_NETWORKREADER_H
#define WEBC_COMPILER_NETWORKREADER_H

#include "IFileReader.h"
#include "StringReader.h"

#include <string>
#include <boost/asio/thread_pool.hpp>

class NetworkReader : public IFileReader {
public:
    explicit NetworkReader(std::string url);

    int getchar() override;

    int seek() override;

    unsigned int getLineNo() override;

    unsigned int getCCol() override;

private:
    int download();

    bool isContentReady();

private:
    std::string url;
    std::unique_ptr<StringReader> string_reader_ptr;
    static int retry_times;
    volatile int download_status;
    boost::asio::thread_pool thread_pool{};
};


#endif //WEBC_COMPILER_NETWORKREADER_H
