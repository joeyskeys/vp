#pragma once

#include "mesh.h"

#include <cstring>

using namespace std;

const BLOCK_SIZE = 2048;

template <typename T, int size>
class Block 
{
public:
    Block();
    ~Block();
    Block(const Block<T, size>& b);
    Block& operator=(const Block<T, size>& b);
    Block(const Block<T, size>&& b);
    Block<T, s>& operator=(Block<T, size>&& b);

    void*  getCurrent();
    void*  getNext();

private:
    int current;
    T *buffer;
};

template <typename T, int size>
Block<T, size>::Block():
    current(-1),
    buffer(nullptr)
{
    buffer = (T*)(malloc(size));
}

template <typename T, int size>
Block<T, size>::~Block()
{
    free(buffer);
}

template <typename T, int size>
Block<T, size>::Block(const Block& b):
    current(b.current),
{
    memcpy(buffer, b.buffer, sizeof(T) * size);
}

template <typename T, int size>
Block<T, size>& Block<T, size>::operator=(const Block<T, size>& b)
{
    current = b.current;
    memcpy(buffer, b.buffer, sizeof(T) * size);

    return *this;
}

template <typename T, int size>
Block(const Block<T, size>&& b):
    current(b.current);
{
    if (buffer)
        free(buffer);
    buffer = b.buffer;
    b.buffer = nullptr;
}

template <typename T, int size>
Block<T, s>& Block<T, size>::operator=(const Block<T, size>&& b)
{
    current = b.current;
    if (buffer)
        free(buffer);
    buffer = b.buffer;
    b.buffer = nullptr;

    return *this;
}

template <typename T, int size>
void* Block<T, size>::getCurrent()
{
    if (current < 0)
        return nullptr;

    return &buffer[current];
}

template <typename T, int size>
void* Block<T, size>::getNext()
{
    if (current < size - 1)
    {
        current++;
        return &buffer[current];
    }
    else
    {
        return nullptr;
    }
}

typedef Block<MeshObj, BLOCK_SIZE> MeshBlock;