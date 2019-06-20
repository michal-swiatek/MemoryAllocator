#ifndef __LINEAR_ALLOCATOR__
#define __LINERA_ALLOCATOR__

#include <iostream>

#include "Allocator.h"

namespace MEM {

    class LinearAllocator : public Allocator
    {
    private:
        void* currPos = nullptr;

    public:
        LinearAllocator() { }
        LinearAllocator(size_t chunkSize, void* memoryChunk) : Allocator(chunkSize, memoryChunk), currPos(memoryChunk) { }
        ~LinearAllocator() { memoryChunk = currPos = nullptr; }

        //
        //  Memory allocation
        //

        inline void* allocate(size_t sizeInBytes, uint8_t alignment = 4) final
        {
            uint8_t adjustment = Align::AlignForwardAdjustment(currPos, alignment);

            if (allocatedMemory + sizeInBytes + adjustment > chunkSize)  return nullptr;

            allocatedMemory += sizeInBytes + adjustment;
            #ifdef DEBUG_MODE
            numberOfAllocations++;
            #endif

            char* alignedMemory = reinterpret_cast<char*>(currPos) + adjustment;
            currPos = reinterpret_cast<void*>(alignedMemory + sizeInBytes);

            return reinterpret_cast<void*>(alignedMemory);
        }

        inline void  deallocate(void* ptr) final
        {
            std::cerr << "Use clearMemory() instead of deallocate for linear allocator!\n";
        }

        void clearMemory()
        {
            currPos = memoryChunk;

            allocatedMemory = 0;
            #ifdef DEBUG_MODE
            numberOfAllocations = 0;
            #endif
        }
    };
}
#endif // __LINEAR_ALLOCATOR__
