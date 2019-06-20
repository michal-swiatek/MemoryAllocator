#ifndef __POOL_ALLOCATOR_TEST__
#define __POOL_ALLOCATOR_TEST__

#include <iostream>

#include "Timer.h"

#include "../Memory/PoolAllocator.h"

template <typename T>
class PoolAllocatorTest
{

private:
    MEM::PoolAllocator<T> poolAllocator;
    Timer timer;

    void* tab[500000];

    double mallocTotalTime = 0.0;
    double allocatorTotalTime = 0.0;

    //  Tests

    //  3 * 50Mb -> 150Mb
    //  10 * 2Mb -> 20Mb
    //  30 * 1Mb -> 30Mb
    //  200Mb

    //  100 * 500kB -> 50Mb
    //  500 * 100kB -> 50Mb
    //  10,000 * 1kB -> 10Mb
    //  110Mb

    //  100,000 * 50B -> 500kB, 0.5Mb
    //  100,000 * 1B -> 0.1Mb
    //  0,6Mb

    void testInternal(size_t numberOfAllocations)
    {
        double elapsedTime;
        size_t objectSize = sizeof(T);

        //      Malloc
        timer.reset();
        for (size_t i = 0; i < numberOfAllocations; i++)
            tab[i] = malloc(objectSize);

        elapsedTime = timer.elapsed();
        std::cout << "Malloc " <<  objectSize << "B, " << numberOfAllocations << " allocations: " << (double)(elapsedTime / 1000000.f) << "s" << '\n';
        mallocTotalTime += elapsedTime;

        for (size_t i = 0; i < numberOfAllocations; i++)
            free(tab[i]);

        //      Allocator
        timer.reset();
        for (size_t i = 0; i < numberOfAllocations; i++) {
            tab[i] = poolAllocator.allocate(sizeof(T), alignof(T));
        }

        elapsedTime = timer.elapsed();
        std::cout << "Allocator " << objectSize << "B, " << numberOfAllocations << " allocations: " << (double)(elapsedTime / 1000000.f) << "s" << '\n';
        allocatorTotalTime += elapsedTime;

        for (size_t i = 0; i < numberOfAllocations; i++)
            poolAllocator.deallocate(tab[i]);

        std::cout << "\n";
    }

public:
    PoolAllocatorTest()
    {
        poolAllocator = MEM::PoolAllocator<T>(50 * MB, malloc(50 * MB));
    }

    ~PoolAllocatorTest()
    {
        free(poolAllocator.getMemoryChunk());
    }

    void test()
    {
        testInternal(1000);
        testInternal(500000);

        std::cout << "Total malloc allocation time: " << mallocTotalTime / 1000000.f << "s" << '\n';
        std::cout << "Total allocator allocation time: " << allocatorTotalTime /1000000.f << "s" << '\n';
        std::cout << "\n\n\n";
    }
};

#endif // __POOL_ALLOCATOR_TEST__

