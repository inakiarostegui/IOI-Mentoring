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

	void Init(std::span<std::byte>&& memory_buffer, const unsigned& chunk_size_in_bytes);

	void* Allocate();

	void Free(void* ptr);

	bool IsChunkFree(void* ptr) const;

	bool IsChunkPtrValid(void* ptr) const;

	void Clear();

	size_t GetBufferSize() const
	{
		return m_buffer.size();
	}

	size_t GetChunkSize() const
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
	std::span<std::byte> m_buffer{};
	unsigned m_chunk_size = 0u;

	std::byte* m_free_list_head = nullptr;		// we could make this an unsigned for distance from start of buffer
};





