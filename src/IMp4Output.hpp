#ifndef __I_OUTPUT_WRITTER_HPP__
#define __I_OUTPUT_WRITTER_HPP__

#include <string>
#include <memory>

#include "Mp4Box.hpp"

class IMp4Output
{
public:
    // Virtual destructor for proper cleanup in derived classes
    virtual ~IMp4Output() = default;

    virtual void writeError(const std::string &text) = 0;
    virtual void writeBox(const Mp4Box::BoxType &boxType, const uint32_t &boxSize) = 0;
    virtual bool writeData(std::istream &dataStream) = 0;
};

#endif // __I_OUTPUT_WRITTER_HPP__