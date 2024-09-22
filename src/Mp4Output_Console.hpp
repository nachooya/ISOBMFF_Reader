#include "IMp4Output.hpp"
#include "IXmlParser.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

class Mp4Ooutput_Console : public IMp4Output
{
public:
    static std::shared_ptr<IMp4Output> New(const std::shared_ptr<IXmlParser> &pXmlParser)
    {
        return std::shared_ptr<IMp4Output>(new Mp4Ooutput_Console(pXmlParser));
    }

    void writeError(const std::string &text) override
    {
        std::cout << text << std::endl;
    }

    void writeBox(const Mp4Box::BoxType &boxType, const uint64_t &boxSize) override
    {
        std::cout << getCurrentTimestamp() << " Found box of type: " << Mp4Box::getBoxTypeString(boxType) << " and  size: " << boxSize << std::endl;
    }

    bool writeData(std::istream &dataStream) override
    {
        if (m_pXmlParser)
        {
            std::cout << getCurrentTimestamp() << " Trying to extract embeded images asuming XML: " << std::endl;
            return m_pXmlParser->readXmlFromStream(dataStream);
        }
        else
        {

            std::cout << getCurrentTimestamp() << " Content of MDAT box is: ";
            while (!dataStream.eof())
            {
                char c;
                dataStream.read(&c, 1);
                std::cout << c;
            }
            std::cout << std::endl;
        }

        return true;
    }

private:
    std::shared_ptr<IXmlParser> m_pXmlParser;

    Mp4Ooutput_Console(const std::shared_ptr<IXmlParser> &pXmlParser) : m_pXmlParser(pXmlParser) {}

    static std::string getCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
                                now.time_since_epoch()) %
                            1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S")
            << '.' << std::setw(3) << std::setfill('0') << milliseconds.count();

        return oss.str();
    }
};