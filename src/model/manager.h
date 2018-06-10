#pragma once

#include "block.h"

#include <vector>
#include <utility>

using namespace std;

template <typename T>
class Manager
{
private:
    Manager();
    ~Manager();
    Manager(const Manager<T>& b);
    Manager& operator=(const Manager<T>& b);

public:
    static Manager<T>* getInstance();
    void*  getNext();

private:
    vector<T> blk_array;
    T   *current_blk;
};

template <typename T>
Manager<T>::Manager()
{
    T first_blk;
    blk_array.push_back(move(first_blk));
    current_blk = &blk_array[0];
}

template <typename T>
Manager<T>::~Manager()
{

}

template <typename T>
Manager<T>* Manager<T>::getInstance()
{
    static Manager<T> instance;
    return &instance;
}

template <typename T>
void* Manager<T>::getNext()
{
    void *next = current_blk->getNext();
    
    if (!next)
    {
        T new_blk;
        blk_array.push_back(move(new_blk));
        current_blk = &blk_array.back();
        next = current_blk->getNext();
    }

    return next;
}

typedef Manager<MeshBlock> MeshManager;
typedef Manager<LightBlock> LightManager;
