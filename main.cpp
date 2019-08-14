#include "Testing/LinearAllocatorTest.h"

#include "Testing/PoolAllocatorTest.h"
#include "Testing/Timer.h"
#include "Testing/StackAllocatorTest.h"
#include <iostream>

#include "Memory/StackAllocator.h"

class Temp1
{
public:
    int data;

    Temp1(int d) : data(d) { }
    ~Temp1() { }
};

class Temp2 : public Temp1
{
public:
    double data2[100];

    Temp2(int d1) : Temp1(d1) { }
    ~Temp2() { }
};

int main()
{
//    LinearAllocatorTest test1;
//
//    test1.test();

    PoolAllocatorTest<Temp2> test2;

    test2.test();

//    StackAllocatorTest test3;
//
//    test3.test();

    return 0;
}
