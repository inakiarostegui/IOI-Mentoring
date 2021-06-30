/***************************************************************************//**
 * @filename UT_StackAllocator.cpp
 * @brief	 Contains the stack allocator unit test function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "StackAllocator.h"
#include "AllocatorTestClass.h"

namespace UT
{
    namespace Allocator
    {
        bool stack_init()
        {
            StackAllocator sa;
            std::byte buffer[48];
            sa.Init(buffer);

            return sa.GetBufferSize() == 48u;
        }

        bool stack_allocate_0()
        {
            StackAllocator sa;
            std::byte buffer[48];
            sa.Init(buffer);

            sa.Allocate(20, 4);

            return sa.GetOffset() == 20 + sizeof(StackAllocator::StackAllocationFooter) + (20 % 4);
        }

        bool stack_allocate_1()
        {
            StackAllocator sa;
            std::byte buffer[48];
            sa.Init(buffer);

            sa.Allocate(20, 8);
            sa.Allocate(30);

            return sa.GetOffset() == 20 + sizeof(StackAllocator::StackAllocationFooter) + (20 % 8);
        }

        bool stack_free_0()
        {
            StackAllocator sa;
            std::byte buffer[64];
            sa.Init(buffer);

            sa.Allocate(20);
            sa.Allocate(20);
            sa.Free();

            return sa.GetOffset() == 20 + sizeof(StackAllocator::StackAllocationFooter);
        }

        bool stack_free_1()
        {
            StackAllocator sa;
            std::byte buffer[48];
            sa.Init(buffer);

            sa.Free();

            return sa.GetBufferSize() == 48;
        }

        bool stack_clear()
        {
            StackAllocator sa;
            std::byte buffer[48];
            sa.Init(buffer);

            sa.Clear();

            return sa.GetOffset() == 0u && sa.GetBufferSize() == 48;
        }

        bool stack_prod()
        {
            StackAllocator sa;
            std::byte buffer_0[sizeof(AllocatorTestClass) * 8];
            sa.Init(buffer_0);

            AllocatorTestClass* data_0 = new (sa.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(1.2, 8);
            AllocatorTestClass* data_1 = new (sa.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(1.5, 10);

            sa.Free();

            AllocatorTestClass* data_2 = new (sa.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(2.2, 5);
            AllocatorTestClass* data_3 = new (sa.Allocate(sizeof(AllocatorTestClass), 4)) AllocatorTestClass(2.6, 2);

            return *data_0 == AllocatorTestClass(1.2, 8) && data_1 == data_2 && *data_2 == AllocatorTestClass(2.2, 5) && *data_3 == AllocatorTestClass(2.6, 2);
        }
    }
}