#include "Testing/LinearAllocatorTest.h"

#include "Testing/PoolAllocatorTest.h"
#include <iostream>

struct TestStruct
{
    double t1;
    int t2;
    int t3;
};

int main()
{
//    LinearAllocatorTest test1;
//
//    test1.test();

    PoolAllocatorTest<TestStruct> test2;

    test2.test();

    return 0;
}
