#include "CppUTest/TestHarness.h"
#include "Mp4Output_Console.hpp"
#include "Mp4Reader.hpp"

class Mp4Output_Test : public IMp4Output
{
public:
    void writeError(const std::string &text) override
    {
        m_errors++;
    }

    void writeBox(const Mp4Box::BoxType &boxType, const uint64_t &boxSize) override
    {
        m_boxes++;
    }

    bool writeData(std::istream &dataStream) override
    {
        m_data++;
        while (!dataStream.eof())
        {
            char c;
            dataStream.read(&c, 1);
        }
        return true;
    }

    uint32_t m_errors = 0;
    uint32_t m_boxes = 0;
    uint32_t m_data = 0;
};

// Create a test group
TEST_GROUP(Mp4ReaderTestGroup)
{
    std::shared_ptr<Mp4Output_Test> outputWritter = nullptr;
    std::shared_ptr<Mp4Reader> reader = nullptr;

    const std::string filePath = "../tests/assets/text0.mp4";

    // Setup method, runs before each test case
    void setup()
    {
        outputWritter = std::make_shared<Mp4Output_Test>();
        reader = Mp4Reader::New(filePath, outputWritter);
    }

    // Teardown method, runs after each test case
    void teardown()
    {
    }
};

TEST(Mp4ReaderTestGroup, TestMp4Reader)
{
    CHECK(reader->readFile());
    LONGS_EQUAL(0, outputWritter->m_errors);
    LONGS_EQUAL(8, outputWritter->m_boxes);
    LONGS_EQUAL(1, outputWritter->m_data);
}

// Additional test cases can be added here
