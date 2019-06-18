#ifndef __LINEAR_ALLOCATOR__
#define __LINERA_ALLOCATOR__

#include <iostream>

#include "Allocator.h"

namespace MEM {

    class LinearAllocator : public Allocator
    {
    private:
        void* currPos = nullptr;

        //
        //  Memory allocation
        //

        void* allocate(size_t sizeInBytes, uint8_t alignment = 4) override
        {
            uint8_t adjustment = Align::AlignForwardAdjustment(currPos, alignment);

            if (allocatedMemory + sizeInBytes + adjustment > chunkSize)  return nullptr;

            allocatedMemory += sizeInBytes + adjustment;
            numberOfAllocations++;

            char* alignedMemory = reinterpret_cast<char*>(currPos) + adjustment;
            currPos = reinterpret_cast<void*>(alignedMemory + sizeInBytes);

            return reinterpret_cast<void*>(alignedMemory);
        }

        void  deallocate(void* ptr)
        {
            std::cerr << "Use clearMemory() instead of deallocate for linear allocator!\n";
        }

    public:
        LinearAllocator() { }
        LinearAllocator(size_t chunkSize, void* memoryChunk) : Allocator(chunkSize, memoryChunk), currPos(memoryChunk) { }
        ~LinearAllocator() { memoryChunk = currPos = nullptr; }

        void clearMemory()
        {
            currPos = memoryChunk;

            allocatedMemory = 0;
            numberOfAllocations = 0;
        }
    };
}
#endif // __LINEAR_ALLOCATOR__
