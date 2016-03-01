#include <utilities/unicode_file_reader.hpp>
#include <locale>
#include <fstream>
#include <sstream>

namespace jdb {

std::wstring UnicodeFileReader::readUTF8(const std::string& filepath)
{
    std::wifstream file(filepath.c_str());
    std::wstringstream fileStream;
    fileStream << file.rdbuf();
    file.close();

    return fileStream.str();
}

} // namespace jdb
