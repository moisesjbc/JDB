#ifndef UNICODE_FILE_READER_HPP
#define UNICODE_FILE_READER_HPP

#include <string>

namespace jdb {

class UnicodeFileReader
{
    public:
        static std::wstring readUTF8(const std::string& filepath, const std::string& locale = "es_ES.UTF8");
};

} // namespace jdb


#endif
// UNICODE_FILE_READER_HPP
