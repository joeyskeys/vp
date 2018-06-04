#pragma once

#include <cstdlib>
#include <cstring>
#include <utility>

using namespace std;

template <typename T>
class Cache {
public:
	Cache();
	~Cache();
	Cache(const Cache<T>& b);
	Cache& operator=(const Cache<T>& b);
	Cache(const Cache<T>&& b);
	Cache& operator=(Cache<T>&& b);

	void fillData(unsigned int c, T *d);

public:
	size_t size;
	size_t offset;
	T *data;
};

template <typename T>
Cache<T>::Cache():
	size(0), offset(0), data(nullptr)
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
Cache<T>::Cache(const Cache<T>&& b):
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
void Cache<T>::fillData(unsigned int c, T *d)
{
	size = c * sizeof(T);
	data = (T*)malloc(size);
	memcpy(data, d, size);
}

typedef Cache<int> Cachei;
typedef Cache<float> Cachef;