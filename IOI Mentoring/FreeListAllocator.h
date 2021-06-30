/***************************************************************************//**
 * @filename FreeListAllocator.h
 * @brief	 Contains the free list allocator class header.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "IAllocator.h"

class FreeListAllocator : public IAllocator
{
public:
	struct FreeListFreeHeader
	{
		FreeListFreeHeader(const unsigned& chunk_size = 0u, FreeListFreeHeader* free_list_next = nullptr) : m_free_list_next(free_list_next), m_chunk_size(chunk_size)
		{	}

		FreeListFreeHeader(const FreeListFreeHeader& flfh)
		{
			m_free_list_next = flfh.m_free_list_next;
			m_chunk_size = flfh.m_chunk_size;
		}

		bool operator==(const FreeListFreeHeader& other)
		{
			return m_free_list_next == other.m_free_list_next && other.m_chunk_size == other.m_chunk_size;
		}

		FreeListFreeHeader* m_free_list_next = nullptr;
		unsigned m_chunk_size = 0u;
	};

	struct FreeListAllocHeader
	{
		FreeListAllocHeader(const unsigned& chunk_size = 0u) : m_chunk_size(chunk_size)
		{	};

		unsigned m_chunk_size = 0u;
	};

	enum class e_AllocType { e_firstfit, e_bestfit };

	FreeListAllocator() = default;

	FreeListAllocator(e_AllocType&& alloc_type) : m_alloc_type(alloc_type)
	{	}
	void Init(std::span<std::byte>&& memory_buffer);

	void* Allocate(unsigned size_in_bytes);

	void Free(void* ptr);

	void Clear();

	bool IsChunkPtrValid(void* ptr, FreeListFreeHeader** prev_free_chunk = nullptr);

	size_t GetBufferSize() const
	{
		return m_buffer.size();
	}

	std::list<FreeListFreeHeader*> GetFreeChunks() const
	{
		std::list<FreeListFreeHeader*> result;
		FreeListFreeHeader* it = m_free_list_head;
		while (it != nullptr)
		{
			result.push_back(it);
			it = it->m_free_list_next;
		}

		return result;
	}

	e_AllocType GetAllocType() const
	{
		return m_alloc_type;
	}

	void SetAllocType(e_AllocType&& alloc_type)
	{
		m_alloc_type = alloc_type;
	}

private:
	bool AreChunksAdjacent(FreeListFreeHeader* chunk_0, FreeListFreeHeader* chunk_1) const;

	void* AllocateFirstFit(const unsigned& size_in_bytes);
	void* AllocateBestFit(const unsigned& size_in_bytes);

	void* AllocateAtChunk(unsigned size_in_bytes, FreeListFreeHeader* prev_free_chunk);


	typedef void* (FreeListAllocator::*AllocateFn)(const unsigned&);
	AllocateFn m_alloc_fns[2] { &FreeListAllocator::AllocateFirstFit, &FreeListAllocator::AllocateBestFit };

	e_AllocType m_alloc_type = e_AllocType::e_firstfit;

	FreeListFreeHeader* m_free_list_head = nullptr;

	std::span<std::byte> m_buffer{};
};

// We cant do inheritance because, what if the user wants to change the alloc type at runtime?
/*
class FreeListAllocatorBestFit : public FreeListAllocator
{
public:
	void* Allocate(const unsigned size_in_bytes);
};

class FreeListAllocatorFirstFit : public FreeListAllocator
{
public:
	void* Allocate(const unsigned size_in_bytes);
};
*/




