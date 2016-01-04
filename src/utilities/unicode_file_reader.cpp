#include "unicode_file_reader.hpp"
#include <locale>
#include <fstream>
#include <sstream>

namespace jdb {

std::wstring UnicodeFileReader::readUTF8(const std::string& filepath, const std::string& locale)
{
    std::wifstream file(filepath.c_str());
    file.imbue(std::locale(locale.c_str()));
    std::wstringstream fileStream;
    fileStream << file.rdbuf();
    file.close();

    return fileStream.str();
}

} // namespace jdb
