#include <memory>
#include <fstream>

using auto_buffer = std::unique_ptr<char[]>;

size_t getFileSize(std::ifstream& f);
auto_buffer readAll(std::string& filepath);
auto_buffer readAll(std::ifstream& f);
