#include "utils.h"

static size_t getFileSize(ifstream& f)
{
	f.seekg(0, ios::beg);
	streampos pos = f.tellg();
	f.seekg(0, ios::end);
	size_t size = f.tellg() - pos;
	f.seekg(0, ios::beg);
    return size;
}

auto_buffer readAll(std::string& filepath)
{
	ifstream f(filepath, ios::in | ios::binary);
	if (!f.good())
		return nullptr;

	size_t size = getFileSize(filepath);
	auto_buffer buf{new char[size + 1]};
	f.read(buf.get(), size);
	auto_buffer[size] = 0;
	f.close();

	return buf;
}