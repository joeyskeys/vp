#include "utils.h"

size_t getFileSize(std::ifstream& f)
{
	f.seekg(0, std::ios::beg);
    std::streampos pos = f.tellg();
	f.seekg(0, std::ios::end);
	size_t size = f.tellg() - pos;
	f.seekg(0, std::ios::beg);
    return size + 1;
}

AutoBuffer readAll(std::string& filepath)
{
    std::ifstream f(filepath, std::ios::in | std::ios::binary);
	if (!f.good())
		return nullptr;

    AutoBuffer buf;
    buf = readAll(f);
	f.close();

	return buf;
}

AutoBuffer readAll(std::ifstream& f)
{
    size_t size = getFileSize(f);
    AutoBuffer buf{new char[size]};
    f.read(buf.get(), size);
    char *buf_ptr = buf.get();
    buf_ptr[size] = 0;

    return buf;
}
