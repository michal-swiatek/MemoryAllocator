#ifndef __LINEAR_ALLOCATOR_TEST__
#define __LINEAR_ALLOCATOR_TEST__

#include "Timer.h"

#include "../Memory/LinearAllocator.h"

class LinearAllocatorTest
{

private:
    MEM::LinearAllocator* linearAllocator;
    Timer timer;

    double mallocTotalTime = 0.0;
    double allocatorTotalTime = 0.0;

    void* testArray[100000];

    void testInternal(size_t memorySize, std::string sufix, size_t numberOfAllocations)
    {
        double elapsedTime;

        //      Malloc
        timer.reset();
        for (size_t i = 0; i < numberOfAllocations; i++)
            testArray[i] = malloc(memorySize);

        elapsedTime = timer.elapsed();
        std::cout << "Malloc " <<  memorySize / (sufix == "MB" ? 1024 * 1024 : (sufix == "kB" ? 1024 : 1)) << sufix << ", " << numberOfAllocations << " allocations: " << (double)(elapsedTime / 1000000.f) << "s" << '\n';
        mallocTotalTime += elapsedTime;

        //      Cleanup

        timer.reset();
        for (size_t i = 0; i < numberOfAllocations; i++)
            free(testArray[i]);

        elapsedTime = timer.elapsed();
        std::cout << "Free: " << elapsedTime / 1000000.f << "ms\n";

        //      Allocator
        timer.reset();
        for (size_t i = 0; i < numberOfAllocations; i++)
            testArray[i] = linearAllocator->allocate(memorySize);

        elapsedTime = timer.elapsed();
        std::cout << "Allocator " << memorySize / (sufix == "MB" ? 1024 * 1024 : (sufix == "kB" ? 1024 : 1)) << sufix << ", " << numberOfAllocations << " allocations: " << (double)(elapsedTime / 1000000.f) << "s" << '\n';
        allocatorTotalTime += elapsedTime;

        //      Cleanup

        timer.reset();
        linearAllocator->clearMemory();

        elapsedTime = timer.elapsed();
        std::cout << "ClearMemory: " << elapsedTime / 1000000.f << "ms\n";

        std::cout << "\n";
    }

public:
    LinearAllocatorTest()
    {
        linearAllocator = new MEM::LinearAllocator(160 * MB, malloc(160 * MB));
    }

    ~LinearAllocatorTest()
    {
        free(linearAllocator->getMemoryChunk());
        delete linearAllocator;
    }

    void test()
    {
        testInternal(50 * MB, "MB", 3);
        testInternal(2 * MB, "MB", 10);
        testInternal(1 * MB, "MB", 30);

        testInternal(500 * kB, "kB", 100);
        testInternal(100 * kB, "kB", 500);
        testInternal(kB, "kB", 10000);

        testInternal(4, "B", 100000);
        testInternal(1, "B", 100000);

        std::cout << "Total malloc allocation time: " << mallocTotalTime / 1000000.f << "s" << '\n';
        std::cout << "Total allocator allocation time: " << allocatorTotalTime /1000000.f << "s" << '\n';
        std::cout << "\n\n\n";
    }
};

#endif // __LINEAR_ALLOCATOR_TEST__
