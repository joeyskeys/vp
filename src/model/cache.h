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

    void clear();
	void reserve(unsigned int c);
	void enlarge(float ratio=1.618f);
	void fillData(unsigned int c, T *d);
	void appendData(T* e, unsigned int cnt);
    void copyData(T* e, unsigned int cnt);
	T*	 useNext();

	inline unsigned int getIdxOfPtr(T *ptr) { return ptr - data; }
	inline T*			getPtrOfIdx(unsigned int idx) { return data + idx; }
	inline int			getCount() { return size / sizeof(T); }
	inline void			deleteIdx(int idx) { data[idx] = data[getCount() - 1]; size -= sizeof(T); }
	inline void			deletePtr(T *ptr) { *ptr = data[getCount() - 1]; size -= sizeof(T); }
	inline void			deleteIdxWithCnt(int idx, int cnt) { memcpy(data + idx, data + getCount() - cnt, cnt * sizeof(T)); }
	inline void			deletePtrWithCnt(T *ptr, int cnt) { memcpy(ptr, data + getCount() - cnt, cnt * sizeof(T)); }
    inline void         setData(T& e, unsigned int idx) { data[idx] = e; }

    T& operator[] (unsigned int idx);
    const T& operator[] (unsigned int idx) const;

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
	size(0), offset(10 * sizeof(T)), data((T*)malloc(10 * sizeof(T)))
{

}

template <typename T>
Cache<T>::~Cache()
{
	if (data)
    {
		free(data);
    }
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
void Cache<T>::clear()
{
    size = 0;
}

template<typename T>
void Cache<T>::reserve(unsigned int c)
{
	size_t new_size = c * sizeof(T);
	if (capability >= new_size)
		return;

	T *tmp = (T*)malloc(new_size);
	if (data)
	{
		memcpy(tmp, data, size);
		free(data);
	}
	data = tmp;
    size = new_size;
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
		float ratio = static_cast<float>(new_size) / capability;
		if (ratio < 1.5f)
			enlarge();
		else
			enlarge(ratio + 0.5f);
	}

	char *tmp = (char*)data;
	memcpy(tmp + size, e, stream_size);
	size = new_size;
}

template<typename T>
void Cache<T>::copyData(T *e, unsigned int cnt)
{
    unsigned int stream_size = cnt * sizeof(T);
    if (stream_size > capability)
    {
        float ratio = static_cast<float>(stream_size) / capability;
        if (ratio < 1.5f)
            enlarge();
        else
            enlarge(ratio + 0.5f);
    }

    char *tmp = (char*)data;
    memcpy(tmp, e, stream_size);
    size = stream_size;
}

template<typename T>
T* Cache<T>::useNext()
{
	if (size >= capability)
		enlarge();

	char *tmp = (char*)data;
	tmp += size;
	size += sizeof(T);
	return (T*)tmp;
}

template<typename T>
T& Cache<T>::operator[] (unsigned int idx)
{
    return data[idx];       
}

template<typename T>
const T& Cache<T>::operator[] (unsigned int idx) const
{
    return data[idx];
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
        // cache destructor isn't virtual to keep struct size
        // do dealloc here will cause double free
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

        return *this;
	}

	inline const char* c_str()
	{
		return data;
	}
};
