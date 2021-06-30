/***************************************************************************//**
 * @filename UT_LinearAllocator.cpp
 * @brief	 Contains the linear allocator unit test function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "LinearAllocator.h"
#include "AllocatorTestClass.h"

namespace UT
{
    namespace Allocator
    {
        bool linear_init()
        {
            LinearAllocator la;
            std::byte buffer[48];
            la.Init(buffer);

            return la.GetBufferSize() == 48;
        }

        bool linear_allocate_0()
        {
            LinearAllocator la;
            std::byte buffer[48]; 
            la.Init(buffer);

            la.Allocate(20, 4);

            return la.GetOffset() == 20 + (20 % 4);
        }

        bool linear_allocate_1()
        {
            LinearAllocator la;
            std::byte buffer[48];
            la.Init(buffer);

            la.Allocate(20, 8);
            la.Allocate(30);

            return la.GetOffset() == 20 + (20 % 8);
        }

        bool linear_free_0()
        {
            LinearAllocator la;
            std::byte buffer[48];
            la.Init(buffer);

            la.Allocate(20);
            la.Free();

            return la.GetOffset() == 0 && la.GetBufferSize() == 48;
        }

        bool linear_prod()
        {
            LinearAllocator la;
            std::byte buffer[sizeof(AllocatorTestClass) * 8];
            la.Init(buffer);

            AllocatorTestClass* data_0 = new (la.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(1.2, 8);
            AllocatorTestClass* data_1 = new (la.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(1.5, 10);

            la.Free();

            AllocatorTestClass* data_2 = new (la.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(2.2, 5);
            AllocatorTestClass* data_3 = new (la.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(2.6, 2);

            return la.GetOffset() == sizeof(AllocatorTestClass) * 2 && *data_2 == AllocatorTestClass(2.2, 5) && *data_3 == AllocatorTestClass(2.6, 2);
        }
    }
}