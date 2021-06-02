/***************************************************************************//**
 * @filename PoolAllocator.h
 * @brief	 Contains the stack allocator class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "IAllocator.h"

class PoolAllocator : public IAllocator
{
public:
	~PoolAllocator()
	{
		Reset();
	}

	std::byte** Init(const unsigned chunk_size_in_bytes, const unsigned chunks_amount/*std::span<std::byte>* memory_buffer*/);

	void* Allocate();

	void Free(void* ptr);

	bool IsChunkFree(void* ptr) const;

	bool IsChunkPtrValid(void* ptr) const;

	void Clear();

	void Reset();

	void PrintData(const bool print_contents) const;

	unsigned GetBufferSize() const
	{
		return m_chunks_amount * m_chunk_size;
	}

	unsigned GetChunkSize() const
	{
		return m_chunk_size;
	}

	std::vector<unsigned> GetFreeChunks() const
	{
		return m_free_chunk_buffer_dists;
	}

private:
	std::byte* m_buffer = nullptr;
	unsigned m_chunks_amount = 0u;
	unsigned m_chunk_size = 0u;
	std::vector<unsigned> m_free_chunk_buffer_dists;
};





