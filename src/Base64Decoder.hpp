#include <string>
#include <fstream>
#include <iostream>

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

bool base64DecodeToFile(const std::string_view &encoded_string, const std::string &outputFilePath)
{

    size_t in_len = encoded_string.size();
    if (in_len % 4 != 0)
    {
        std::cerr << "Invalid Base64 string: len: " << in_len << std::endl;
        return false;
    }

    size_t padding = 0;
    if (encoded_string[in_len - 1] == '=')
        padding++;
    if (encoded_string[in_len - 2] == '=')
        padding++;

    size_t out_len = (in_len / 4) * 3 - padding;

    std::ofstream out(outputFilePath, std::ios::binary);
    if (!out)
    {
        std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
        return false;
    }

    for (size_t i = 0; i < in_len;)
    {
        uint32_t a = i < in_len ? base64_chars.find(encoded_string[i++]) : 0;
        uint32_t b = i < in_len ? base64_chars.find(encoded_string[i++]) : 0;
        uint32_t c = i < in_len ? base64_chars.find(encoded_string[i++]) : 0;
        uint32_t d = i < in_len ? base64_chars.find(encoded_string[i++]) : 0;

        uint32_t temp = (a << 18) + (b << 12) + (c << 6) + d;

        if (out.tellp() < out_len)
            out.put((temp >> 16) & 0xFF);
        if (out.tellp() < out_len)
            out.put((temp >> 8) & 0xFF);
        if (out.tellp() < out_len)
            out.put(temp & 0xFF);
    }

    out.close();

    return true;
}