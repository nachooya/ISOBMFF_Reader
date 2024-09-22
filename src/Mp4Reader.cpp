#include "Mp4Box.hpp"
#include "Mp4Reader.hpp"
#include "BoundedStream.hpp"
#include <bit>

bool isLittleEndian()
{
    /*
     * Available in C++20
     * return std::endian::native == std::endian::little;
     */
    uint16_t number = 1;
    return *reinterpret_cast<uint8_t *>(&number) == 1;
}

uint32_t toLittleEndian32(uint32_t bigEndian)
{
    return (bigEndian >> 24) & 0xff |  // Move byte 3 to byte 0
           (bigEndian >> 8) & 0xff00 | // Move byte 2 to byte 1
           (bigEndian & 0xff00) << 8 | // Move byte 1 to byte 2
           (bigEndian & 0xff) << 24;   // Move byte 0 to byte 3
}

uint64_t toLittleEndian64(uint64_t bigEndian)
{
    return (bigEndian >> 56) & 0xff |   // Move byte 7 to byte 0
           (bigEndian >> 48) & 0xff00 | // Move byte 6 to byte 1
           (bigEndian >> 40) & 0xff00 | // Move byte 5 to byte 2
           (bigEndian >> 32) & 0xff00 | // Move byte 4 to byte 3
           (bigEndian >> 24) & 0xff00 | // Move byte 3 to byte 4
           (bigEndian >> 16) & 0xff00 | // Move byte 2 to byte 5
           (bigEndian >> 8) & 0xff00 |  // Move byte 1 to byte 6
           (bigEndian & 0xff00) << 8 |  // Move byte 0 to byte 7
           (bigEndian & 0xff) << 56;    // Move byte 7 to byte 0
}

std::shared_ptr<Mp4Reader> Mp4Reader::New(const std::string &pFilePath, const std::shared_ptr<IMp4Output> &pOutputWritter)
{
    return std::shared_ptr<Mp4Reader>(new Mp4Reader(pFilePath, pOutputWritter));
}

Mp4Reader::Mp4Reader(const std::string &filePath, const std::shared_ptr<IMp4Output> &pOutputWritter)
{
    m_filePath = filePath;
    m_outputWritter = pOutputWritter;
    m_isLittleEndian = isLittleEndian();
}

bool Mp4Reader::readUnit32t(std::ifstream &file, uint32_t &value) const
{
    file.read(reinterpret_cast<char *>(&value), sizeof(value));
    // Check if the file is a valid MP4 file
    if (file.gcount() != 4)
    {
        return false;
    }
    else
    {
        if (m_isLittleEndian)
        {
            value = toLittleEndian32(value);
        }
    }
    return true;
}

bool Mp4Reader::readUnit64t(std::ifstream &file, uint64_t &value) const
{
    file.read(reinterpret_cast<char *>(&value), sizeof(value));
    // Check if the file is a valid MP4 file
    if (file.gcount() != 8)
    {
        return false;
    }
    else
    {
        if (m_isLittleEndian)
        {
            value = toLittleEndian64(value);
        }
    }
    std::cout << "Read 64-bit value: " << value << std::endl;
    return true;
}

bool Mp4Reader::readFile() const
{
    // For every box first 4 bytes are the size of the box
    // The first 4 bytes of the box are the size of the box in bytes, including the size of the header.
    // The size of the box is the size of the box in bytes, excluding the size of the header.
    // If the size of the box is less than 8 bytes, the size of the box is stored in the first 4 bytes.
    // If the size of the box is 8 bytes or greater, the size of the box is stored in the first 8 bytes.

    std::ifstream file(m_filePath);
    if (!file)
    {
        m_outputWritter->writeError("Error: Could not open file " + m_filePath);
        return false;
    }
    else
    {

        while (true)
        {

            uint32_t boxSize32;
            uint64_t boxSize64;

            if (!readUnit32t(file, boxSize32))
            {
                if (file.eof())
                {
                    return true;
                }
                else
                {
                    m_outputWritter->writeError("Error: Invalid MP4 file. Error reading box size.");
                    return false;
                }
            }
            else
            {
                boxSize64 = boxSize32;
            }

            uint32_t boxTypeCode;
            if (readUnit32t(file, boxTypeCode) == false)
            {
                m_outputWritter->writeError("Error: Invalid MP4 file. Error reading box type.");
                return false;
            }

            // Large size case: read 64-bit size
            if (boxSize32 == 1)
            {
                if (!readUnit64t(file, boxSize64))
                {
                    m_outputWritter->writeError("Error: Invalid MP4 file. Error reading box size.");
                    return false;
                }
            }

            auto boxType = Mp4Box::checkBoxCode(boxTypeCode);

            if (boxType == Mp4Box::TYPE_UNKNOWN)
            {
                printf("Unknown box type: 0x%X == %.*s\n", boxTypeCode, 4, (char *)&boxTypeCode);
                m_outputWritter->writeError("Error: Unknown box type: " + std::to_string(boxTypeCode));
                return false;
            }
            else
            {
                m_outputWritter->writeBox(boxType, boxSize64);

                // a. A box of type MOOF only contains other boxes
                // b. A box of type TRAF only contains other boxes
                // c. All other boxes don’t contain other boxes.
                // 3. If the box of type MDAT is found, extract and print the content of that box. It can be assumed that the
                // content is a UTF-8 encoded XML string.

                if (boxType == Mp4Box::MOOF)
                {
                }
                else if (boxType == Mp4Box::TRAF)
                {
                }
                else
                {
                    if (boxType == Mp4Box::MDAT)
                    {
                        auto boundedStream = BoundedStream(file, boxSize64 - 8);
                        m_outputWritter->writeData(boundedStream);
                    }
                    else
                    {
                        file.seekg(boxSize64 - 8, std::ios::cur);
                    }
                }
            }
        }
    }
    return false;
}
