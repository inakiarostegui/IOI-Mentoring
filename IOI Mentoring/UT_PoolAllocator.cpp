/***************************************************************************//**
 * @filename UT_PoolAllocator.cpp
 * @brief	 Contains the pool allocator unit test function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "PoolAllocator.h"
#include "AllocatorTestClass.h"

namespace UT
{
    namespace Allocator
    {
        bool pool_init()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);

            return pa.GetBufferSize() == 8u * 12u;
        }

        bool pool_allocate_0()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);
            void* chunk_0 = pa.Allocate();
            void* chunk_1 = pa.Allocate();

            return pa.GetFreeChunkAmount() == 10 && !pa.IsChunkFree(chunk_0) && !pa.IsChunkFree(chunk_1);
        }

        bool pool_allocate_1()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);

            for (unsigned i = 0u; i < 12; i++)
                pa.Allocate();

            void* chunk_13 = pa.Allocate();

            return pa.GetFreeChunkAmount() == 0 && chunk_13 == nullptr;
        }

        bool pool_free_0()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);
            void* chunk_0 = pa.Allocate();
            void* chunk_1 = pa.Allocate();
            pa.Free(chunk_0);

            return pa.GetFreeChunkAmount() == 11 && pa.IsChunkFree(chunk_0);
        }

        bool pool_free_1()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);
            void* chunk_0 = pa.Allocate();
            int* temp_ptr = new int;
            pa.Free(static_cast<void*>(temp_ptr));

            return pa.GetFreeChunkAmount() == 11;
        }

        bool pool_free_2()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);
            void* chunk_0 = pa.Allocate();

            pa.Free(static_cast<void*>(&chunk_0 + 2));

            return pa.GetFreeChunkAmount() == 11;
        }

        bool pool_clear()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);
            void* chunk_0 = pa.Allocate();
            pa.Clear();

            return pa.GetFreeChunkAmount() == 12;
        }

        bool pool_reset()
        {
             PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);
            void* chunk_0 = pa.Allocate();
            pa.Reset();

            return *buffer == nullptr;
        }

        bool pool_init_reset()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(8, 12);
            pa.Allocate();
            buffer = pa.Init(8, 12);

            return pa.GetBufferSize() == 8 * 12 && pa.GetChunkSize() == 8u && pa.GetFreeChunkAmount() == 12;
        }

        bool pool_prod()
        {
            PoolAllocator pa;
            std::byte** buffer = pa.Init(sizeof(AllocatorTestClass), 3);

            AllocatorTestClass* data_0 = new (pa.Allocate()) AllocatorTestClass(1.2, 8);
            AllocatorTestClass* data_1 = new (pa.Allocate()) AllocatorTestClass(1.5, 10);
            AllocatorTestClass* data_2 = new (pa.Allocate()) AllocatorTestClass(2.2, 5);

            pa.Free(data_0);
            pa.Free(data_2);

            AllocatorTestClass* data_3 = new (pa.Allocate()) AllocatorTestClass(2.6, 2);

            pa.Free(data_1);

            AllocatorTestClass* data_4 = new (pa.Allocate()) AllocatorTestClass(3.4, 3);
            AllocatorTestClass* data_5 = new (pa.Allocate()) AllocatorTestClass(0.2, 2);

            pa.Free(data_3);

            return pa.GetFreeChunkAmount() == 1 && pa.IsChunkFree(data_3)
                && *data_4 == AllocatorTestClass(3.4, 3) && *data_5 == AllocatorTestClass(0.2, 2);
        }
    }
}