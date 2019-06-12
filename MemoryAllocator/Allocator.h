#ifndef __ALLOCATOR__
#define __ALLOCATOR__

#include <cassert>

//1000 1008 1016
//1    0    0

//void* 1000, char version 0 | int index
//void* 1000, char version 1 | int index

class Allocator
{
protected:
    void* memoryChunk;

    size_t chunkSize;
    size_t allocatedMemory;
    size_t numberOfAllocations;

    //  Copying might lead to errors or memory leaks
    Allocator(const Allocator& rhs) = delete;
    Allocator& operator = (const Allocator& rhs) = delete;

public:
    Allocator(size_t chunkSize, void* memoryChunk)
    {
        this->memoryChunk = memoryChunk;

        this->chunkSize = chunkSize;
        this->allocatedMemory = 0;
        this->numberOfAllocations = 0;
    }

    virtual ~Allocator()
    {
        assert(allocatedMemory == 0 && numberOfAllocations == 0);
    }

    virtual void* allocate(size_t sizeInBytes, size_t alignment = 4) = 0;
    virtual void  deallocate(void* ptr) = 0;

    inline void*  getStart() { return start; }
    inline size_t getMaxSize() const { return maxSize; }
    inline size_t getAllocatedMemory() const { return allocatedMemory; }
    inline size_t getRemainingMemory() const { return maxSize - allocatedMemory; }
    inline size_t getNumberOfAllocations() const { return numberOfAllocations; }
};

namespace

#endif // __ALLOCATOR__
