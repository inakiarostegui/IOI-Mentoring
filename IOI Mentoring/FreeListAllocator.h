/***************************************************************************//**
 * @filename PoolAllocator.h
 * @brief	 Contains the free list allocator class header.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "IAllocator.h"
#include "FreeListMemoryChunk.h"

class FreeListAllocator : public IAllocator
{
public:
	typedef std::list<MemoryChunk>::const_iterator MemoryChunkListIterator;
	typedef std::vector<MemoryChunk>::const_iterator MemoryChunkVectorIterator;

	enum class e_AllocType { e_firstfit, e_bestfit };

	FreeListAllocator() = default;

	FreeListAllocator(e_AllocType&& alloc_type) : m_alloc_type(alloc_type)
	{	}

	~FreeListAllocator()
	{
		Reset();
	}

	std::byte** Init(const unsigned memory_buffer_length_in_bytes/*std::span<std::byte>* memory_buffer*/);

	void* Allocate(const unsigned size_in_bytes);

	void Free(void* ptr);

	void Clear();

	void Reset();

	bool IsChunkPtrValid(void* ptr, MemoryChunkVectorIterator* alloc_chunk_it);

	void PrintData(const bool print_contents = false) const;

	unsigned GetBufferSize() const
	{
		return m_buffer_size;
	}

	std::list<MemoryChunk> GetFreeChunks() const
	{
		return m_free_chunks;
	}

	std::vector<MemoryChunk> GetAllocatedChunks() const
	{
		return m_alloc_chunks;
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
	bool AreChunksAdjancent(const MemoryChunk& chunk_0, const MemoryChunk& chunk_1) const;

	void InsertSortFreeChunk(MemoryChunk&& free_chunk, MemoryChunkListIterator* free_chunk_it);

	MemoryChunk* InsertFreeChunk(MemoryChunk&& free_chunk);

	void ConcatenateChunks(MemoryChunk* modificable_chunk, MemoryChunkListIterator&& other_chunk);

	void FreeAllocatedChunk(MemoryChunkVectorIterator&& alloc_chunk_it, MemoryChunk* new_free_chunk);

	void* AllocateFirstFit(const unsigned size_in_bytes);
	void* AllocateBestFit(const unsigned size_in_bytes);

	void* AllocateAtChunk(const unsigned size_in_bytes, MemoryChunkListIterator&& free_chunk);


	typedef void* (FreeListAllocator::*AllocateFn)(const unsigned);
	AllocateFn m_alloc_fns[2] { &FreeListAllocator::AllocateFirstFit, &FreeListAllocator::AllocateBestFit };

	e_AllocType m_alloc_type = e_AllocType::e_firstfit;

	std::list<MemoryChunk> m_free_chunks;
	std::vector<MemoryChunk> m_alloc_chunks;

	std::byte* m_buffer = nullptr;
	unsigned m_buffer_size = 0u;
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




