#include "utils.h"

size_t getFileSize(std::ifstream& f)
{
	f.seekg(0, ios::beg);
	streampos pos = f.tellg();
	f.seekg(0, ios::end);
	size_t size = f.tellg() - pos;
	f.seekg(0, ios::beg);
    return size + 1;
}

auto_buffer readAll(std::string& filepath)
{
    std::ifstream f(filepath, std::ios::in | std::ios::binary);
	if (!f.good())
		return nullptr;

    auto_buffer buf;
    buf = readAll(f);
	f.close();

	return buf;
}

auto_buffer readAll(std::ifstream& f)
{
    size_t size = getFileSize(f);
    auto_buffer buf{new char[size]};
    f.read(buf.get(), size);
    auto_buffer[size] = 0;

    return buf;
}
