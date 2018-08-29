#include <memory>
#include <fstream>

using AutoBuffer = std::unique_ptr<char[]>;

size_t getFileSize(std::ifstream& f);
AutoBuffer readAll(const std::string& filepath);
AutoBuffer readAll(std::ifstream& f);
