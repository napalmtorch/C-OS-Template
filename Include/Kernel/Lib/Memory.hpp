#pragma once
#include <Kernel/Lib/Types.hpp>

namespace Memory
{
    void* Set(void* dest, int data, uint size);
    void* Copy(void* dest, void* src, uint size);
    void* Move(void* dest, void* src, uint size);
    int Compare(void* p1, void* p2, uint size);
}

// allocate overloads
extern void *operator new(uint64_t size);
extern void *operator new[](uint64_t size);

// delete overloads
extern void operator delete(void *p);
extern void operator delete(void *p, size_t size);
extern void operator delete[](void *p);
extern void operator delete[](void *p, size_t size);