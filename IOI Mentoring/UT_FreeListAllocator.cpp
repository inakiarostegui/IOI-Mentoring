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
			std::byte** buffer = flaff.Init(16);

			return flaff.GetBufferSize() == 16u && flaff.GetFreeChunks().size() == 1u;
		}

		bool freelist_allocate_firstfit_0()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(64);
			flaff.Allocate(12);

			std::list<FreeListAllocator::FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();
			
			return free_chunks.size() == 1 && free_chunks.front()->m_chunk_size == 64 - 12 - 2 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_allocate_firstfit_1()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(32);
			flaff.Allocate(32 - sizeof(FreeListAllocator::FreeListAllocHeader));

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.empty();
		}

		bool freelist_allocate_firstfit_2()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(64);
			flaff.Allocate(25);
			flaff.Allocate(25);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.empty();
		}

		bool freelist_allocate_firstfit_3()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(256);

			void* chunk_to_free_0 = flaff.Allocate(12);
			flaff.Allocate(16);
			void* chunk_to_free_1 = flaff.Allocate(32);
			flaff.Allocate(16);

			flaff.Free(chunk_to_free_0);
			flaff.Free(chunk_to_free_1);

			flaff.Allocate(14);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.size() == 3u && free_chunks.front()->m_chunk_size == 12u && free_chunks.back()->m_chunk_size == 256u - 12 - 16 - 32 - 16 - 5 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_allocate_bestfit_0()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte** buffer = flabf.Init(64);
			flabf.Allocate(12);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flabf.GetFreeChunks();

			return free_chunks.size() == 1 && free_chunks.front()->m_chunk_size == 64 - 12 - 2 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_allocate_bestfit_1()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte** buffer = flabf.Init(64);
			flabf.Allocate(25);
			flabf.Allocate(25);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flabf.GetFreeChunks();

			return free_chunks.empty();
		}

		bool freelist_allocate_bestfit_2()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte** buffer = flabf.Init(256);

			void* chunk_to_free_0 = flabf.Allocate(16);
			flabf.Allocate(12);
			void* chunk_to_free_1 = flabf.Allocate(14);
			flabf.Allocate(12);

			flabf.Free(chunk_to_free_0);
			flabf.Free(chunk_to_free_1);

			flabf.Allocate(14);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flabf.GetFreeChunks();

			return free_chunks.size() == 2u && free_chunks.front()->m_chunk_size == 16u && free_chunks.back()->m_chunk_size == 256u - 16 - 12 - 14 - 12 - 5 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_free_0()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(128);

			void* chunk_to_free_0 = flaff.Allocate(16);
			flaff.Allocate(12);
			void* chunk_to_free_1 = flaff.Allocate(16);
			flaff.Allocate(12);
			void* chunk_to_free_2 = flaff.Allocate(16);

			flaff.Free(chunk_to_free_0);
			flaff.Free(chunk_to_free_1);
			flaff.Free(chunk_to_free_2);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.size() == 3u && free_chunks.front()->m_free_list_next->m_chunk_size == 16u;
		}

		bool freelist_free_1()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(64);
			flaff.Allocate(16);

			int* temp_ptr = new int;
			flaff.Free(static_cast<void*>(temp_ptr));

			return flaff.GetFreeChunks().size() == 1;
		}

		bool freelist_free_2()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(32);

			void* chunk_0 = flaff.Allocate(12);
			flaff.Free(static_cast<void*>(reinterpret_cast<std::byte*>(chunk_0) + 2));	// Undefined Behaviour

			return true;
		}

		bool freelist_clear()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(64);

			void* chunk_to_free = flaff.Allocate(16);
			flaff.Allocate(12);
			flaff.Free(chunk_to_free);

			flaff.Clear();

			return flaff.GetFreeChunks().size() == 1u && flaff.GetBufferSize() == 64u;
		}

		bool freelist_reset()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(32);
			flaff.Allocate(12);

			flaff.Reset();

			return *buffer == nullptr;
		}

		bool freelist_init_reset()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(64);
			flaff.Allocate(12);

			buffer = flaff.Init(32);

			return flaff.GetBufferSize() == 32u;
		}

		bool freelist_prod()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte** buffer = flaff.Init(400);
			std::vector<void*> chunks;

			for (int i = 0; i < 400 / 20; i++)
				chunks.push_back(flaff.Allocate(16));

			for (int i = 0; i < chunks.size(); i += 2)
				flaff.Free(chunks[i]);
			for (int i = 1; i < chunks.size(); i += 2)
				flaff.Free(chunks[i]);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.size() == 1u && free_chunks.front()->m_chunk_size == flaff.GetBufferSize() - sizeof(FreeListAllocator::FreeListAllocHeader);
		}
	}
}