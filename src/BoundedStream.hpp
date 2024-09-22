#include <fstream>
#include <iostream>
#include <streambuf>

/**
 * Custom streambuf to limit the number of bytes read from a stream
 */
class BoundedStreambuf : public std::streambuf
{
public:
  BoundedStreambuf(std::ifstream &file, std::streamsize maxLength)
      : file(file), maxLength(maxLength), bytesRead(0) {}

protected:
  virtual int_type underflow() override
  {
    if (bytesRead >= maxLength)
    {
      return traits_type::eof(); // End of limit reached
    }
    return file.peek(); // Peek the next character
  }

  virtual std::streamsize xsgetn(char *s, std::streamsize n) override
  {
    if (bytesRead >= maxLength)
    {
      return 0; // No more data to read
    }
    std::streamsize bytesToRead = std::min(n, maxLength - bytesRead);
    file.read(s, bytesToRead);
    std::streamsize bytesReadNow = file.gcount();
    bytesRead += bytesReadNow;
    return bytesReadNow; // Return the number of bytes read
  }

private:
  std::ifstream &file;       // Reference to the wrapped file stream
  std::streamsize maxLength; // Maximum length to read
  std::streamsize bytesRead; // Count of bytes read
};

/**
 * Custom istream to limit the number of bytes read from a stream
 */
class BoundedStream : public std::istream
{
public:
  BoundedStream(std::ifstream &file, std::streamsize length)
      : std::istream(&buffer), buffer(file, length) {}

private:
  BoundedStreambuf buffer; // Custom stream buffer
};
