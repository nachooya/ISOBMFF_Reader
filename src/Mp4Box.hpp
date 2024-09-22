#ifndef __MP4_BOX_HPP__
#define __MP4_BOX_HPP__

#include <cstdint>
#include <string>
#include <array>

// Function to convert a 4-character string to a uint32_t
constexpr uint32_t charStringToHex(const char str[5])
{
    return (static_cast<uint32_t>(str[0]) << 24) |
           (static_cast<uint32_t>(str[1]) << 16) |
           (static_cast<uint32_t>(str[2]) << 8) |
           (static_cast<uint32_t>(str[3]));
}

class Mp4Box
{

public:
    using BoxType = uint32_t;

    static constexpr BoxType TYPE_UNKNOWN = 0;

    static constexpr BoxType MOOF = charStringToHex("moof");
    static constexpr BoxType TRAF = charStringToHex("traf");
    static constexpr BoxType MFHD = charStringToHex("mfhd");
    static constexpr BoxType TFHD = charStringToHex("tfhd");
    static constexpr BoxType MDAT = charStringToHex("mdat");
    static constexpr BoxType FREE = charStringToHex("free");
    static constexpr BoxType TRUN = charStringToHex("trun");
    static constexpr BoxType UUID = charStringToHex("uuid");
    static constexpr BoxType SKIP = charStringToHex("skip");

    static constexpr std::array<BoxType, 9> knowBoxTypes = {
        MOOF, TRAF, MFHD, TFHD, MDAT, FREE, TRUN, UUID, SKIP};

    static constexpr BoxType checkBoxCode(const uint32_t &boxCode)
    {
        // Loop through the constexpr array for lookup
        for (const auto &knownBoxType : knowBoxTypes)
        {
            if (boxCode == knownBoxType)
            {
                return knownBoxType;
            }
        }
        return TYPE_UNKNOWN;
    }

    static std::string getBoxTypeString(const BoxType &boxType)
    {
        std::string result = {((char *)&boxType)[3], ((char *)&boxType)[2], ((char *)&boxType)[1], ((char *)&boxType)[0]};
        return result;
    }
};

#endif // __MP4_BOX_HPP__