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
			std::byte buffer[16];
			flaff.Init(buffer);

			return flaff.GetBufferSize() == 16 && flaff.GetFreeChunks().size() == 1;
		}

		bool freelist_allocate_firstfit_0()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[64];
			flaff.Init(buffer);

			flaff.Allocate(12);

			std::list<FreeListAllocator::FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();
			
			return free_chunks.size() == 1 && free_chunks.front()->m_chunk_size == 64 - 12 - 2 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_allocate_firstfit_1()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[32];
			flaff.Init(buffer);

			flaff.Allocate(32 - sizeof(FreeListAllocator::FreeListAllocHeader));

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.empty();
		}

		bool freelist_allocate_firstfit_2()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[64];
			flaff.Init(buffer);

			flaff.Allocate(25);
			flaff.Allocate(25);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.empty();
		}

		bool freelist_allocate_firstfit_3()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[256];
			flaff.Init(buffer);

			void* chunk_to_free_0 = flaff.Allocate(12);
			flaff.Allocate(16);
			void* chunk_to_free_1 = flaff.Allocate(32);
			flaff.Allocate(16);

			flaff.Free(chunk_to_free_0);
			flaff.Free(chunk_to_free_1);

			flaff.Allocate(14);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.size() == 3 && free_chunks.front()->m_chunk_size == 12 && free_chunks.back()->m_chunk_size == 256 - 12 - 16 - 32 - 16 - 5 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_allocate_bestfit_0()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte buffer[64];
			flabf.Init(buffer);

			flabf.Allocate(12);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flabf.GetFreeChunks();

			return free_chunks.size() == 1 && free_chunks.front()->m_chunk_size == 64 - 12 - 2 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_allocate_bestfit_1()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte buffer[64];
			flabf.Init(buffer);

			flabf.Allocate(25);
			flabf.Allocate(25);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flabf.GetFreeChunks();

			return free_chunks.empty();
		}

		bool freelist_allocate_bestfit_2()
		{
			FreeListAllocator flabf(FreeListAllocator::e_AllocType::e_bestfit);
			std::byte buffer[256];
			flabf.Init(buffer);

			void* chunk_to_free_0 = flabf.Allocate(16);
			flabf.Allocate(12);
			void* chunk_to_free_1 = flabf.Allocate(14);
			flabf.Allocate(12);

			flabf.Free(chunk_to_free_0);
			flabf.Free(chunk_to_free_1);

			flabf.Allocate(14);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flabf.GetFreeChunks();

			return free_chunks.size() == 2 && free_chunks.front()->m_chunk_size == 16 && free_chunks.back()->m_chunk_size == 256 - 16 - 12 - 14 - 12 - 5 * sizeof(FreeListAllocator::FreeListAllocHeader);
		}

		bool freelist_free_0()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[128];
			flaff.Init(buffer);

			void* chunk_to_free_0 = flaff.Allocate(16);
			flaff.Allocate(12);
			void* chunk_to_free_1 = flaff.Allocate(16);
			flaff.Allocate(12);
			void* chunk_to_free_2 = flaff.Allocate(16);

			flaff.Free(chunk_to_free_0);
			flaff.Free(chunk_to_free_1);
			flaff.Free(chunk_to_free_2);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.size() == 3 && free_chunks.front()->m_free_list_next->m_chunk_size == 16;
		}

		bool freelist_free_1()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[64];
			flaff.Init(buffer);

			flaff.Allocate(16);

			int* temp_ptr = new int;
			flaff.Free(static_cast<void*>(temp_ptr));

			return flaff.GetFreeChunks().size() == 1;
		}

		bool freelist_free_2()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[32];
			flaff.Init(buffer);

			void* chunk_0 = flaff.Allocate(12);
			flaff.Free(static_cast<void*>(reinterpret_cast<std::byte*>(chunk_0) + 2));	// Undefined Behaviour

			return true;
		}

		bool freelist_clear()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[64];
			flaff.Init(buffer);

			void* chunk_to_free = flaff.Allocate(16);
			flaff.Allocate(12);
			flaff.Free(chunk_to_free);

			flaff.Clear();

			return flaff.GetFreeChunks().size() == 1 && flaff.GetBufferSize() == 64;
		}

		bool freelist_prod()
		{
			FreeListAllocator flaff(FreeListAllocator::e_AllocType::e_firstfit);
			std::byte buffer[400];
			flaff.Init(buffer);
			std::vector<void*> chunks;

			for (int i = 0; i < 400 / 20; i++)
				chunks.push_back(flaff.Allocate(16));

			for (int i = 0; i < chunks.size(); i += 2)
				flaff.Free(chunks[i]);
			for (int i = 1; i < chunks.size(); i += 2)
				flaff.Free(chunks[i]);

			std::list<FreeListAllocator::FreeListFreeHeader*> free_chunks = flaff.GetFreeChunks();

			return free_chunks.size() == 1 && free_chunks.front()->m_chunk_size == flaff.GetBufferSize() - sizeof(FreeListAllocator::FreeListAllocHeader);
		}
	}
}