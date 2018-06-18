#pragma once

//#include "mesh.h"
//#include "light.h"

#include <cstring>

using namespace std;

const int BLOCK_SIZE = 64;
const int LARGE_BLOCK_SIZE = 1024;

struct MeshObj;
typedef struct MeshObj MeshObj;
struct LightObj;
typedef struct LightObj LightObj;
struct ShaderProgramObj;
typedef struct ShaderProgramObj ShaderProgramObj;

template <typename T, int size>
class Block 
{
public:
    Block();
    ~Block();
    Block(const Block<T, size>& b);
    Block& operator=(const Block<T, size>& b);
    Block(Block<T, size>&& b);
    Block<T, size>& operator=(Block<T, size>&& b);

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
    current(b.current)
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
Block<T,size>::Block(Block<T, size>&& b)
{
	current = b.current;
    buffer = b.buffer;
    b.buffer = nullptr;
}

template <typename T, int size>
Block<T, size>& Block<T, size>::operator=(Block<T, size>&& b)
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

typedef Block<MeshObj, LARGE_BLOCK_SIZE> MeshBlock;
typedef Block<LightObj, BLOCK_SIZE> LightBlock;
typedef Block<ShaderProgramObj, BLOCK_SIZE> ShaderProgramBlock;
