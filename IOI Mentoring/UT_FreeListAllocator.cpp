/***************************************************************************//**
 * @filename UT_FreeListAllocator.cpp
 * @brief	 Contains the free list allocator unit test function 
 *			 implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "FreeListAllocator.h"
#include "AllocatorTestClass.h"

namespace UT
{
	namespace Allocator
	{
		bool freelist_init()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);

			return flaff.GetBufferSize() == 8u && flaff.GetFreeChunks().size() == 1u;
		}

		bool freelist_allocate_firstfit_0()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);
			flaff.Allocate(1);

			std::vector<MemoryChunk> alloc_chunks = flaff.GetAllocatedChunks();
			std::list<MemoryChunk> free_chunks = flaff.GetFreeChunks();

			return alloc_chunks.size() == 1u && alloc_chunks[0].m_dist_to_buffer == 0u && alloc_chunks[0].m_size == 1u && 
				   free_chunks.size() == 1 && free_chunks.front().m_dist_to_buffer == 1u && free_chunks.front().m_size == 7u;
		}

		bool freelist_allocate_firstfit_1()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);
			flaff.Allocate(8);
			flaff.Allocate(1);

			std::vector<MemoryChunk> alloc_chunks = flaff.GetAllocatedChunks();
			std::list<MemoryChunk> free_chunks = flaff.GetFreeChunks();

			return alloc_chunks.size() == 1u && free_chunks.empty();
		}

		bool freelist_allocate_firstfit_2()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);

			void* chunk_to_free = flaff.Allocate(4);
			flaff.Allocate(2);
			flaff.Free(chunk_to_free);

			flaff.Allocate(2);

			std::vector<MemoryChunk> alloc_chunks = flaff.GetAllocatedChunks();
			std::list<MemoryChunk> free_chunks = flaff.GetFreeChunks();

			return alloc_chunks.size() == 2u && 
				   free_chunks.size() == 2u && free_chunks.front() == MemoryChunk(2u, 2u) && free_chunks.back() == MemoryChunk(6u, 2u);
		}

		bool freelist_allocate_bestfit_0()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte** buffer = flabf.Init(8);
			flabf.Allocate(1);

			std::vector<MemoryChunk> alloc_chunks = flabf.GetAllocatedChunks();
			std::list<MemoryChunk> free_chunks = flabf.GetFreeChunks();

			return alloc_chunks.size() == 1u && alloc_chunks[0].m_dist_to_buffer == 0u && alloc_chunks[0].m_size == 1u &&
				   free_chunks.size() == 1 && free_chunks.front().m_dist_to_buffer == 1u && free_chunks.front().m_size == 7u;
		}

		bool freelist_allocate_bestfit_1()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte** buffer = flabf.Init(8);
			flabf.Allocate(8);
			flabf.Allocate(1);

			std::vector<MemoryChunk> alloc_chunks = flabf.GetAllocatedChunks();
			std::list<MemoryChunk> free_chunks = flabf.GetFreeChunks();

			return alloc_chunks.size() == 1u && free_chunks.empty();
		}

		bool freelist_allocate_bestfit_2()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte** buffer = flabf.Init(8);

			void* chunk_to_free = flabf.Allocate(4);
			flabf.Allocate(2);
			flabf.Free(chunk_to_free);

			flabf.Allocate(2);

			std::vector<MemoryChunk> alloc_chunks = flabf.GetAllocatedChunks();
			std::list<MemoryChunk> free_chunks = flabf.GetFreeChunks();

			return alloc_chunks.size() == 2u &&
				   free_chunks.size() == 1u && free_chunks.front() == MemoryChunk(0u, 4u);
		}

		bool freelist_free_0()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(5);

			void* chunk_to_free_0 = flaff.Allocate(1);
			flaff.Allocate(1);
			void* chunk_to_free_1 = flaff.Allocate(1);
			flaff.Allocate(1);
			void* chunk_to_free_2 = flaff.Allocate(1);

			flaff.Free(chunk_to_free_0);
			flaff.Free(chunk_to_free_1);
			flaff.Free(chunk_to_free_2);

			std::vector<MemoryChunk> alloc_chunks = flaff.GetAllocatedChunks();
			std::list<MemoryChunk> free_chunks = flaff.GetFreeChunks();

			return alloc_chunks.size() == 2u &&
				   free_chunks.size() == 3u && free_chunks.front() == MemoryChunk(0u, 1u) && free_chunks.back() == MemoryChunk(4u, 1u);
		}

		bool freelist_free_1()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);
			flaff.Allocate(4);

			int* temp_ptr = new int;
			flaff.Free(static_cast<void*>(temp_ptr));

			return flaff.GetAllocatedChunks().size() == 1;
		}

		bool freelist_free_2()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);

			void* chunk_0 = flaff.Allocate(4);
			flaff.Free(static_cast<void*>(&chunk_0 + 2));

			return flaff.GetAllocatedChunks().size() == 1;
		}

		bool freelist_clear()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(3);

			void* chunk_to_free = flaff.Allocate(1);
			flaff.Allocate(2);
			flaff.Free(chunk_to_free);

			flaff.Clear();

			return flaff.GetAllocatedChunks().empty() && flaff.GetFreeChunks().size() == 1u && flaff.GetBufferSize() == 3u;
		}

		bool freelist_reset()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);
			flaff.Allocate(4);

			flaff.Reset();

			return *buffer == nullptr;
		}

		bool freelist_init_reset()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(8);
			flaff.Allocate(4);

			buffer = flaff.Init(4);

			return flaff.GetBufferSize() == 4u;
		}

		bool freelist_prod()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(100);

			for (int i = 0; i < 100; i++)
				flaff.Allocate(1);

			for (int i = 99; i > 0; i -= 2)
				flaff.Free(*buffer + ptrdiff_t(i));

			for (int i = 98; i >= 0; i -= 2)
				flaff.Free(*buffer + ptrdiff_t(i));

			std::list<MemoryChunk> free_chunks = flaff.GetFreeChunks();

			return free_chunks.size() == 1u && free_chunks.front().m_size == flaff.GetBufferSize() && free_chunks.front().m_dist_to_buffer == 0u &&
				   flaff.GetAllocatedChunks().empty();
		}
	}
}