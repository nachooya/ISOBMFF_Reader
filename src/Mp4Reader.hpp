#ifndef __MP4_READER_HPP__
#define __MP4_READER_HPP__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "IMp4Output.hpp"

class Mp4Reader
{
public:
    static std::shared_ptr<Mp4Reader> New(const std::string &pFilePath, const std::shared_ptr<IMp4Output> &pOutputFormat);

    // Function to read the entire contents of the file
    bool readFile() const;

private:
    // Constructor to initialize the file path
    explicit Mp4Reader(const std::string &pFilePath, const std::shared_ptr<IMp4Output> &pOutputFormat);
    bool readUnit32t(std::ifstream &file, uint32_t &value) const;

    bool m_isLittleEndian;
    std::string m_filePath;
    std::shared_ptr<IMp4Output> m_outputWritter;
};

#endif // __MP4_READER_HPP__
