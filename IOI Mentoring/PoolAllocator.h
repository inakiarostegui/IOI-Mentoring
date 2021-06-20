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
	struct PoolAllocationHeader
	{
		PoolAllocationHeader(std::byte* free_list_next = nullptr) : m_free_list_next(free_list_next)
		{	};

		std::byte* m_free_list_next = nullptr;
	};

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

	// For debug & test purposes
	unsigned GetFreeChunkAmount() const
	{
		if (m_free_list_head == nullptr)
			return 0u;

		unsigned free_chunk_count = 0u;
		std::byte* it = m_free_list_head;
		while (it != nullptr)
		{
			it = reinterpret_cast<PoolAllocationHeader*>(it)->m_free_list_next;
			free_chunk_count++;
		}
		return free_chunk_count;
	}

private:
	std::byte* m_buffer = nullptr;
	unsigned m_chunks_amount = 0u;
	unsigned m_chunk_size = 0u;

	std::byte* m_free_list_head = nullptr;		// we could make this an unsigned for distance from start of buffer
};





