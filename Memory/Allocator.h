#ifndef __ALLOCATOR__
#define __ALLOCATOR__

#include <cassert>
#include <cstdlib>
#include <utility>
#include <cstdint>

namespace MEM {

    class Allocator
    {
    protected:
        void* memoryChunk;

        size_t chunkSize;
        size_t allocatedMemory; //  Number of memory allocated during allocator lifetime
        #if DEBUG
        size_t numberOfAllocations;
        #endif

    public:
        Allocator(size_t chunkSize, void* memoryChunk)
        {
            assert(chunkSize > 0 && memoryChunk);

            this->memoryChunk = memoryChunk;

            this->chunkSize = chunkSize;
            this->allocatedMemory = 0;
            #if DEBUG
            this->numberOfAllocations = 0;
            #endif
        }

        virtual ~Allocator()
        {
            #if DEBUG
            assert(allocatedMemory == 0 && numberOfAllocations == 0);
            #endif
        }

        //
        //  Memory allocation
        //

        inline virtual void* allocate(size_t sizeInBytes, uint8_t alignment = 4) = 0;
        inline virtual void  deallocate(void* ptr) = 0;

        //  Use this instead of new and delete

        template <typename T, typename... Args>
        inline T* New(const Args&... args)
        {
            return new (allocate(sizeof(T), alignof(T))) T(std::move(args)...);
        }

        template <typename T>
        inline void Delete(T* object)
        {
            object->~T();
            deallocate(object);
        }

        //  Getters & setters

        inline void*  getMemoryChunk() { return memoryChunk; }
        inline size_t getMemoryChunkSize() const { return chunkSize; }

        inline size_t getAllocatedMemory() const { return allocatedMemory; }
        inline size_t getRemainingMemory() const { return chunkSize - allocatedMemory; }
        #if DEBUG
        inline size_t getNumberOfAllocations() const { return numberOfAllocations; }
        #endif

        //  Copying allocators might cause errors
        Allocator(const Allocator& rhs)                 = delete;
        Allocator& operator = (const Allocator& rhs)    = delete;
    };

    //  Helper functions

    namespace Align {

        inline void* AlignForward(void* ptr, uint8_t alignment)
        {
            return reinterpret_cast<void*>((reinterpret_cast<int>(ptr) + alignment - 1) & ~(alignment - 1));
        }

        inline uint8_t AlignForwardAdjustment(const void* ptr, uint8_t alignment)
        {
            uint8_t adjustment = alignment - static_cast<uint8_t>(reinterpret_cast<int>(ptr) & (alignment - 1));

            return adjustment == alignment ? 0 : adjustment;
        }

        inline uint8_t AlignForwardAdjustmentWithHeader(const void* ptr, uint8_t alignment, uint8_t header)
        {
            uint8_t adjustment = AlignForwardAdjustment(ptr, alignment);

            //  Header is always a multiple of 2 (data is always represented as a power of 2)
            //  Alignment is always a power of 2
            if (adjustment < header)
                adjustment += alignment * ((header - adjustment - 1) / alignment + 1);

            return adjustment;
        }
    }
}

#endif // __ALLOCATOR__
