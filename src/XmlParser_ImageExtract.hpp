#include <sstream>
#include <string>
#include <vector>
#include <pugixml.hpp>
#include "IXmlParser.hpp"
#include "Base64Decoder.hpp"

class XmlParser_ImageExtract : public IXmlParser
{
public:
    bool readXmlFromStream(std::istream &stream)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load(stream);

        if (!result)
        {
            std::cerr << "XML parsed with errors: " << result.description() << std::endl;
            return false;
        }

        // Search for smpte:image tags
        // with this attributes xml:id="image003" imagetype="PNG" encoding="Base64"
        for (auto const &imageNode : doc.select_nodes("//smpte:image"))
        {
            // Get the xml:id attribute
            auto idAttr = imageNode.node().attribute("xml:id");

            if (!idAttr)
            {
                std::cerr << "Error: smpte:image tag does not have an xml:id attribute" << std::endl;
                continue;
            }

            // Get the imagetype attribute
            auto imageTypeAttr = imageNode.node().attribute("imagetype");

            if (!imageTypeAttr)
            {
                std::cerr << "Error: smpte:image tag does not have an imagetype attribute" << std::endl;
                continue;
            }

            // Get the encoding attribute
            auto encodingAttr = imageNode.node().attribute("encoding");

            if (!encodingAttr)
            {
                std::cerr << "Error: smpte:image tag does not have an encoding attribute" << std::endl;
                continue;
            }

            // Get the content of the smpte:image tag
            auto imageContent = imageNode.node().text().as_string();

            if (std::string_view(encodingAttr.value()) == "Base64")
            {
                auto content = std::string_view(imageContent);
                // Trim the leading and trailing carriage returns and line feeds
                auto base64Image = std::string_view(content.data() + 1, content.size() - 2);
                std::string decodedImage = std::string(idAttr.value()) + "." + std::string(imageTypeAttr.value());
                if (base64DecodeToFile(std::string_view(base64Image), decodedImage))
                {
                    std::cout << "Decoded image content: " << decodedImage << std::endl;
                }
                else
                {
                    std::cerr << "Error: Could not decode image: " << decodedImage << std::endl;
                }
            }
            else
            {
                std::cerr << "Found smpte:image tag with unknown encoding: " << encodingAttr.value() << std::endl;
            }
        }

        return true;
    }
};