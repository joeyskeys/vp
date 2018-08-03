#include <memory>

using auto_buffer = std::unique_ptr<char[]>;

std::unique_ptr<char[]> readAll(std::string& filepath);