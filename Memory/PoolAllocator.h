#ifndef __POOL_ALLOCATOR__
#define __POOL_ALLOCATOR__

#include "Allocator.h"

namespace MEM {

    template <typename T>
    class PoolAllocator : public Allocator
    {
    private:
        void** freeList;

        inline void** movePointer(void** ptr, size_t offset)
        {
            return reinterpret_cast<void**>(reinterpret_cast<char*>(ptr) + offset);
        }

    public:
        PoolAllocator(size_t chunkSize, void* memoryChunk) : Allocator(chunkSize, memoryChunk)
        {
            //  empty blocks hold pointer to next memory block
            assert(sizeof(T) >= sizeof(void*));

            uint8_t adjustment = Align::AlignForwardAdjustment(memoryChunk, alignof(T));
            freeList = movePointer(reinterpret_cast<void**>(memoryChunk), adjustment);

            size_t objectSize = sizeof(T);
            size_t numberOfObjects = (chunkSize - adjustment) / objectSize;

            //  Initialization of free list
            T* listPtr = reinterpret_cast<T*>(freeList);

            for (size_t i = 0; i < numberOfObjects - 1; ++i, ++listPtr)
                *reinterpret_cast<void**>(listPtr) = reinterpret_cast<void*>(listPtr + 1);

            *movePointer(freeList, (numberOfObjects - 1) * objectSize) = nullptr;
        }

        ~PoolAllocator()
        {
            freeList = nullptr;
        }

        inline void* allocate(size_t sizeInBytes, uint8_t alignment = 4) final
        {
            assert(sizeInBytes == sizeof(T) && alignment == alignof(T));

            if (!freeList)  return nullptr;

            void* ptr = reinterpret_cast<void*>(freeList);
            freeList = reinterpret_cast<void**>(*freeList);

            allocatedMemory += sizeInBytes;
            #ifdef DEBUG
            numberOfAllocations++;
            #endif

            return ptr;
        }

        inline void deallocate(void* ptr) final
        {
            *reinterpret_cast<void**>(ptr) = reinterpret_cast<void*>(freeList);
            freeList = reinterpret_cast<void**>(ptr);

            allocatedMemory -= sizeof(T);
            #ifdef DEBUG
            numberOfAllocations--;
            #endif
        }

        //  Copying allocators might cause errors
        PoolAllocator(const PoolAllocator& rhs)                 = delete;
        PoolAllocator& operator = (const PoolAllocator& rhs)    = delete;
    };
}

#endif // __POOL_ALLOCATOR__

