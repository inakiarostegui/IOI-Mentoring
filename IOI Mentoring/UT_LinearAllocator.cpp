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
            std::byte** buffer = la.Init(48);

            return la.GetBufferSize() == 48u;
        }

        bool linear_allocate_0()
        {
            LinearAllocator la;
            std::byte** buffer = la.Init(48);
            la.Allocate(20, 4u);

            return la.GetOffset() == 20u + (20u % 4u);
        }

        bool linear_allocate_1()
        {
            LinearAllocator la;
            std::byte** buffer = la.Init(48);
            la.Allocate(20, 8u);
            la.Allocate(30);

            return la.GetOffset() == 20u + (20u % 8u);
        }

        bool linear_free_0()
        {
            LinearAllocator la;
            std::byte** buffer = la.Init(48);
            la.Allocate(20);
            la.Free();

            return la.GetOffset() == 0u && la.GetBufferSize() == 48u;
        }

        bool linear_reset()
        {
            LinearAllocator la;
            std::byte** buffer = la.Init(48);
            la.Allocate(20);
            la.Reset();

            return *buffer == nullptr;
        }

        bool linear_init_reset()
        {
            LinearAllocator la;
            std::byte** buffer = la.Init(48);
            la.Allocate(20);
            buffer = la.Init(36);

            return la.GetBufferSize() == 36u;
        }

        bool linear_prod()
        {
            LinearAllocator la;

            std::byte** buffer = la.Init(sizeof(AllocatorTestClass) * 8);

            AllocatorTestClass* data_0 = new (la.Allocate(sizeof(AllocatorTestClass), 4u)) AllocatorTestClass(1.2, 8);
            AllocatorTestClass* data_1 = new (la.Allocate(sizeof(AllocatorTestClass), 4u)) AllocatorTestClass(1.5, 10);

            la.Free();

            AllocatorTestClass* data_2 = new (la.Allocate(sizeof(AllocatorTestClass), 4u)) AllocatorTestClass(2.2, 5);
            AllocatorTestClass* data_3 = new (la.Allocate(sizeof(AllocatorTestClass), 4u)) AllocatorTestClass(2.6, 2);

            return la.GetOffset() == sizeof(AllocatorTestClass) * 2 && *data_2 == AllocatorTestClass(2.2, 5) && *data_3 == AllocatorTestClass(2.6, 2);
        }
    }
}