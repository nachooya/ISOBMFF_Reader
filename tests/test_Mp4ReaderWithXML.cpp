#include "CppUTest/TestHarness.h"
#include "Mp4Output_Console.hpp"
#include "XmlParser_ImageExtract.hpp"
#include "Mp4Reader.hpp"
#include <filesystem>

// Create a test group
TEST_GROUP(Mp4ReaderTestGroupWithXML)
{
    std::shared_ptr<Mp4Reader> reader = nullptr;

    const std::string filePath = "../tests/assets/text0.mp4";

    // Setup method, runs before each test case
    void setup()
    {
        auto xmlParser = std::make_shared<XmlParser_ImageExtract>();
        auto outputWritter = Mp4Ooutput_Console::New(xmlParser);
        reader = Mp4Reader::New(filePath, outputWritter);
    }

    // Teardown method, runs after each test case
    void teardown()
    {
    }
};

TEST(Mp4ReaderTestGroupWithXML, TestMp4Reader)
{
    CHECK(reader->readFile());
    // Check images were extracted
    CHECK(std::filesystem::exists("./image001.PNG"));
    CHECK(std::filesystem::exists("./image002.PNG"));
    CHECK(std::filesystem::exists("./image003.PNG"));
}

// Additional test cases can be added here
