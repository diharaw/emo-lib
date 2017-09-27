#pragma once

#include <common.hpp>
#include <istream>

class Membuf : public std::basic_streambuf<char>
{
public:
    Membuf(const uint8_t *p, size_t l);
};

class MemoryStream : public std::istream
{
public:
    MemoryStream(const uint8_t *p, size_t l);
    
private:
    Membuf _buffer;
};
