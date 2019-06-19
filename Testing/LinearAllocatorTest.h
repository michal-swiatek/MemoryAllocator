#ifndef __LINEAR_ALLOCATOR_TEST__
#define __LINEAR_ALLOCATOR_TEST__

#include "Timer.h"

#include "../Memory/LinearAllocator.h"

class LinearAllocatorTest
{

private:
    MEM::LinearAllocator linearAllocator;
    Timer timer;

    double mallocTotalTime = 0.0;
    double allocatorTotalTime = 0.0;

    void* testArray[100000];

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
            testArray[i] = linearAllocator.allocateRaw(memorySize);

        elapsedTime = timer.elapsed();
        std::cout << "Allocator " << memorySize / (sufix == "MB" ? 1024 * 1024 : (sufix == "kB" ? 1024 : 1)) << sufix << ", " << numberOfAllocations << " allocations: " << (double)(elapsedTime / 1000000.f) << "s" << '\n';
        allocatorTotalTime += elapsedTime;

        //      Cleanup

        timer.reset();
        linearAllocator.clearMemory();

        elapsedTime = timer.elapsed();
        std::cout << "ClearMemory: " << elapsedTime / 1000000.f << "ms\n";

        std::cout << "\n";
    }

public:
    LinearAllocatorTest()
    {
        linearAllocator = MEM::LinearAllocator(160 * MB, malloc(160 * MB));
    }

    ~LinearAllocatorTest()
    {
        free(linearAllocator.getMemoryChunk());
    }

    void test()
    {
//        testInternal(50 * MB, "MB", 3);
//        testInternal(2 * MB, "MB", 10);
//        testInternal(1 * MB, "MB", 30);
//
//        testInternal(500 * kB, "kB", 100);
//        testInternal(100 * kB, "kB", 500);
//        testInternal(kB, "kB", 10000);

        testInternal(4, "B", 100000);
        testInternal(1, "B", 100000);

//        testInternal(16, "B", 10000);
//        testInternal(256, "B", 1000);
//        testInternal(2 * MB, "MB", 50);

        std::cout << "Total malloc allocation time: " << mallocTotalTime / 1000000.f << "s" << '\n';
        std::cout << "Total allocator allocation time: " << allocatorTotalTime /1000000.f << "s" << '\n';
        std::cout << "\n\n\n";
    }
};

#endif // __LINEAR_ALLOCATOR_TEST__
