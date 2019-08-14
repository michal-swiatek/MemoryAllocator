#ifndef __STACK_ALLOCATOR__
#define __STACK_ALLOCATOR__

#include "Allocator.h"

namespace MEM {

    class StackAllocator : public Allocator
    {
    private:
        struct Header
        {
            #ifdef DEBUG
            void* prevAdress;
            #endif // DEBUG

            uint8_t adjustment;
        };

        #ifdef DEBUG
        void* prevPos;
        #endif // DEBUG

        void* currPos;

    public:
        StackAllocator(size_t chunkSize, void* memoryChunk) : Allocator(chunkSize, memoryChunk)
        {
            assert(chunkSize > 0);
            #ifdef DEBUG
            prevPos = nullptr;
            #endif // DEBUG

            currPos = memoryChunk;
        }

        ~StackAllocator()
        {
            #if DEBUG
            prevPos = nullptr;
            #endif

            currPos = nullptr;
        }

        inline void* allocate(size_t sizeInBytes, uint8_t alignment = 4) final
        {
            assert(sizeInBytes > 0);

            uint8_t adjustment = Align::AlignForwardAdjustmentWithHeader(currPos, alignment, sizeof(Header));

            if (allocatedMemory + adjustment + sizeInBytes > chunkSize) return nullptr;

            char* alignedMemory = reinterpret_cast<char*>(currPos) + adjustment;

            Header* header = reinterpret_cast<Header*>(alignedMemory - sizeof(Header));
            header->adjustment = adjustment;
            #ifdef DEBUG
            header->prevAdress = prevPos;
            prevPos = reinterpret_cast<void*>(alignedMemory);
            #endif // DEBUG

            currPos = reinterpret_cast<void*>(alignedMemory + sizeInBytes);
            allocatedMemory += sizeInBytes + adjustment;
            #ifdef DEBUG
            numberOfAllocations++;
            #endif // DEBUG

            return reinterpret_cast<void*>(alignedMemory);
        }

        inline void deallocate(void* ptr) final
        {
            #ifdef DEBUG
            assert(ptr == prevPos);
            #endif

            Header* header = reinterpret_cast<Header*>(reinterpret_cast<char*>(ptr) - sizeof(Header));
            allocatedMemory -= (reinterpret_cast<char*>(currPos) - reinterpret_cast<char*>(ptr)) + header->adjustment;

            currPos = reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) - header->adjustment);

            #ifdef DEBUG
            numberOfAllocations--;
            prevPos = header->prevAdress;
            #endif // DEBUG
        }

        //  Copying allocators might cause errors
        StackAllocator(const StackAllocator& rhs)                 = delete;
        StackAllocator& operator = (const StackAllocator& rhs)    = delete;
    };
}

#endif // __STACK_ALLOCATOR__
