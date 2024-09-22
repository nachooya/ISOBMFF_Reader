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

uint32_t toLittleEndian(uint32_t bigEndian)
{
    return (bigEndian >> 24) & 0xff |  // Move byte 3 to byte 0
           (bigEndian >> 8) & 0xff00 | // Move byte 2 to byte 1
           (bigEndian & 0xff00) << 8 | // Move byte 1 to byte 2
           (bigEndian & 0xff) << 24;   // Move byte 0 to byte 3
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
            value = toLittleEndian(value);
        }
    }
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

            uint32_t boxSize;
            ;
            if (!readUnit32t(file, boxSize))
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

            uint32_t boxTypeCode;
            if (readUnit32t(file, boxTypeCode) == false)
            {
                m_outputWritter->writeError("Error: Invalid MP4 file. Error reading box type.");
                return false;
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
                m_outputWritter->writeBox(boxType, boxSize);

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
                        auto boundedStream = BoundedStream(file, boxSize - 8);
                        m_outputWritter->writeData(boundedStream);
                    }
                    else
                    {
                        file.seekg(boxSize - 8, std::ios::cur);
                    }
                }
            }
        }
    }
    return false;
}
