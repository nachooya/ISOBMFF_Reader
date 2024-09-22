#include "Mp4Reader.hpp"
#include "Mp4Output_Console.hpp"
#ifdef PUGIXML_ENABLED
#include "XmlParser_ImageExtract.hpp"
#endif
#include <iostream>
#include <string>
#include <map>

std::map<std::string, std::string> parseCommandLine(int argc, char *argv[], std::string &filePath)
{
    std::map<std::string, std::string> options;
    filePath = "";

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg[0] == '-' && arg[1] == '-')
        { // Option prefixed with "--"
            std::string value;
            if (i + 1 < argc && argv[i + 1][0] != '-')
            {                      // If next argument is a value
                value = argv[++i]; // Assign the next argument as value
            }
            options[arg] = value;
        }
        else
        {
            filePath = arg; // Treat as file path if it's not an option
        }
    }

    return options;
}

void printUsage(const std::string_view &programName)
{
    std::cout << "Usage:" << programName << " [options] <file>" << std::endl;
#ifdef PUGIXML_ENABLED
    std::cout << "Options:" << std::endl;
    std::cout << "\t--extract-images  Extract images from the file" << std::endl;
#endif
}

int main(int argc, char *argv[])
{
    std::string filePath;
    auto options = parseCommandLine(argc, argv, filePath);
    bool extractImages = false;
    std::shared_ptr<IXmlParser> xmlParser = nullptr;

#ifdef PUGIXML_ENABLED
    if (options.count("--extract-images"))
    {
        xmlParser = std::make_shared<XmlParser_ImageExtract>();
    }
#endif

    if (filePath.empty())
    {
        printUsage(argv[0]);
        return 1; // Return error if no file path is provided
    }

    auto outputWritter = Mp4Ooutput_Console::New(xmlParser);
    auto reader = Mp4Reader::New(filePath, outputWritter);
    return reader->readFile() ? 0 : 1;
}
