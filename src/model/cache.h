#pragma once

#include <cstdlib>
#include <cstring>
#include <utility>
#include <string>

using namespace std;

template <typename T>
class Cache {
public:
	Cache();
	~Cache();
	Cache(const Cache<T>& b);
	Cache& operator=(const Cache<T>& b);
	Cache(Cache<T>&& b);
	Cache& operator=(Cache<T>&& b);

	void reserve(unsigned int c);
	void enlarge(float ratio=1.618f);
	void fillData(unsigned int c, T *d);
	void appendData(T* e, unsigned int cnt);
	T*	 useNext();

	inline unsigned int getIdxOfPtr(T *ptr) { return ptr - data; }
	inline T*			getPtrOfIdx(unsigned int idx) { return data + idx; }

public:
	unsigned int size;
	union
	{
		unsigned int capability;
		unsigned int offset;
	};
	T *data;
};

template <typename T>
Cache<T>::Cache():
	size(0), offset(10 * sizeof(T)), data(malloc(10 * sizeof(T)))
{

}

template <typename T>
Cache<T>::~Cache()
{
	if (data)
		free(data);
}

template <typename T>
Cache<T>::Cache(const Cache<T>& b):
	size(b.size),
	offset(b.offset)
{
	data = (T*)(malloc(size));
	memcpy(data, b.data, size);
}

template <typename T>
Cache<T>& Cache<T>::operator=(const Cache<T>& b)
{
	if (data && size != b.size)
	{
		free(data);
		data = nullptr;
	}

	size = b.size;
	if (!data)
		data = (T*)(malloc(size));
	memcpy(data, b.data, size);

	return *this;
}

template <typename T>
Cache<T>::Cache(Cache<T>&& b):
	size(b.size),
	offset(b.offset)
{
	if (data)
		free(data);
	data = b.data;
	b.data = nullptr;
}

template<typename T>
Cache<T>& Cache<T>::operator=(Cache<T>&& b)
{
	size = b.size;
	offset = b.offset;
	if (data)
		free(data);
	data = b.data;
	b.data = nullptr;

	return *this;
}

template<typename T>
void Cache<T>::reserve(unsigned int c)
{
	new_size = c * sizeof(T);
	if (size >= new_size)
		return;

	T *tmp = (T*)malloc(new_size);
	if (data)
	{
		memcpy(tmp, data, size);
		free(data);
	}
	data = tmp;
}

template<typename T>
void Cache<T>::enlarge(float ratio)
{
	unsigned int new_capability = ratio * capability;
	T *tmp = (T*)malloc(new_capability);
	memcpy(tmp, data, size);
	free(data);
	data = tmp;
	capability = new_capability;
}

template<typename T>
void Cache<T>::fillData(unsigned int c, T *d)
{
	if (data && size != c * sizeof(T))
		free(data);
	
	size = c * sizeof(T);
	data = (T*)malloc(size);
	memcpy(data, d, size);
}

template<typename T>
void Cache<T>::appendData(T *e, unsigned int cnt)
{
	unsigned int stream_size = cnt * sizeof(T);
	unsigned int new_size = size + stream_size;
	if (new_size > capability)
	{
		float ratio = static_cast<float>new_size / capability;
		if (ratio < 1.5f)
			enlarge();
		else
			enlarge(ratio + 0.5f);
	}

	char *tmp = data;
	memcpy(tmp + size, e, stream_size);
	size = new_size;
}

template<typename T>
T* Cache<T>::useNext()
{
	if (size >= capability)
		enlarge();

	char *tmp = data;
	tmp += size;
	size += sizeof(T);
	return tmp;
}

typedef Cache<int> Cachei;
typedef Cache<unsigned int> Cacheu;
typedef Cache<float> Cachef;
typedef Cache<char>	Cachec;

class Str : public Cachec
{
public:
	Str(): Cachec() {}

	Str(const string& cppstr)
	{
		size = cppstr.size();
		data = (char*)malloc(size);
		memcpy(data, cppstr.c_str(), size);
	}

	~Str()
	{
		if (data)
			free(data);
	}

	Str(const Str& rhs): Cachec(rhs) {}

	Str& operator=(const Str& rhs)
	{
		Cachec::operator=(rhs);

		return *this;
	}

	Str(const Str&& rhs): Cachec(move(rhs)) {}

	Str& operator=(const Str&& rhs)
	{
		Cachec::operator=(move(rhs));

		return *this;
	}

	Str& operator=(const string& cppstr)
	{
		if (data && size != cppstr.size())
			free(data);

		size = cppstr.size();
		data = (char*)malloc(size);
		memcpy(data, cppstr.c_str(), size);
	}

	inline const char* c_str()
	{
		return data;
	}
};