#ifndef UNICODE_FILE_READER_HPP
#define UNICODE_FILE_READER_HPP

#include <string>

namespace jdb {

class UnicodeFileReader
{
    public:
        static std::wstring readUTF8(const std::string& filepath);
};

} // namespace jdb


#endif
// UNICODE_FILE_READER_HPP
