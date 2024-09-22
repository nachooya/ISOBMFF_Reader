#ifndef __I_XML_PARSER_HPP__
#define __I_XML_PARSER_HPP__

#include <iostream>

class IXmlParser
{
public:
    virtual bool readXmlFromStream(std::istream &stream) = 0;
};

#endif // __I_XML_PARSER_HPP__