#include <memory_stream.hpp>

Membuf::Membuf(const uint8_t *p, size_t l)
{
    setg((char*)p, (char*)p, (char*)p + l);
}

MemoryStream::MemoryStream(const uint8_t *p, size_t l) : std::istream(&_buffer), _buffer(p, l)
{
    rdbuf(&_buffer);
}
