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
            std::byte** buffer = sa.Init(48);

            return sa.GetBufferSize() == 48u;
        }

        bool stack_allocate_0()
        {
            StackAllocator sa;
            std::byte** buffer = sa.Init(48);
            sa.Allocate(20);

            return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
        }

        bool stack_allocate_1()
        {
            StackAllocator sa;
            std::byte** buffer = sa.Init(48);
            sa.Allocate(20);
            sa.Allocate(30);

            return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
        }

        bool stack_free_0()
        {
            StackAllocator sa;
            std::byte** buffer = sa.Init(48);
            sa.Allocate(20);
            sa.Allocate(20);
            sa.Free();

            return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
        }

        bool stack_free_1()
        {
            StackAllocator sa;
            std::byte** buffer = sa.Init(48);
            sa.Free();

            return sa.GetBufferSize() == 48u;
        }

        bool stack_clear()
        {
            StackAllocator sa;
            std::byte** buffer = sa.Init(48u);
            sa.Clear();

            return sa.GetOffset() == 0u && sa.GetAmountOfBlocks() == 0u && sa.GetBufferSize() == 48u;
        }

        bool stack_reset()
        {
            StackAllocator sa;
            std::byte** buffer = sa.Init(48u);
            sa.Reset();

            return *buffer == nullptr;
        }

        bool stack_init_reset()
        {
            StackAllocator sa;
            std::byte** buffer = sa.Init(48);
            sa.Allocate(20);
            buffer = sa.Init(36);

            return sa.GetBufferSize() == 36u && sa.GetOffset() == 0u && sa.GetAmountOfBlocks() == 0;
        }

        bool stack_prod()
        {
            StackAllocator sa;

            std::byte** buffer = sa.Init(sizeof(AllocatorTestClass) * 8);

            AllocatorTestClass* data_0 = new (sa.Allocate(sizeof(AllocatorTestClass))) AllocatorTestClass(1.2, 8);
            AllocatorTestClass* data_1 = new (sa.Allocate(sizeof(AllocatorTestClass))) AllocatorTestClass(1.5, 10);

            sa.Free();

            AllocatorTestClass* data_2 = new (sa.Allocate(sizeof(AllocatorTestClass))) AllocatorTestClass(2.2, 5);
            AllocatorTestClass* data_3 = new (sa.Allocate(sizeof(AllocatorTestClass))) AllocatorTestClass(2.6, 2);

            return sa.GetAmountOfBlocks() == 3 && *data_0 == AllocatorTestClass(1.2, 8) && data_1 == data_2 && *data_2 == AllocatorTestClass(2.2, 5) && *data_3 == AllocatorTestClass(2.6, 2);
        }
    }
}