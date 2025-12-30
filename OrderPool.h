#pragma once
#include <vector>
#include <stdexcept>
#include "OrderDefs.h"

struct OrderNode
{
    Order order;
    int next = -1; // pointer to next node index
    int prev = -1; // pointer to previous node index
};

class OrderPool
{
private:
    std::vector<OrderNode> store;
    int freehead = 0; // the first empty slot

public:
    OrderPool(int size)
    {
        store.resize(size);
        for (int i = 0; i < size - 1; ++i)
        {
            store[i].next = i + 1;
        }
        store[size - 1].next = -1;
    }

    int allocate()
    {
        if (freehead == -1)
            throw std::runtime_error("pool exhausted.");
        int index = freehead;
        freehead = store[index].next;

        store[index].next = -1;
        store[index].prev = -1;

        return index;
    }

    void deallocate(int index) // just "marking" the index as empty. Actual updation happens through overwriting
    {
        store[index].next = freehead;
        freehead = index;
    }

    OrderNode &get(int index)
    {
        return store[index];
    }
};
