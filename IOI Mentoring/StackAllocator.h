/***************************************************************************//**
 * @filename StackAllocator.h
 * @brief	 Contains the stack allocator class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "IAllocator.h"

class StackAllocator : public IAllocator
{
public:
	struct StackAllocationFooter
	{
		StackAllocationFooter(const size_t& alloc_size) : m_alloc_size(alloc_size)
		{	};

		size_t m_alloc_size;
	};

	void Init(std::span<std::byte>&& memory_buffer);

	void* Allocate(size_t size_in_bytes, const size_t& alignment = 0u);

	void Free();

	void Clear();

	size_t GetOffset() const
	{
		return m_offset;
	}

	size_t GetBufferSize() const
	{
		return m_buffer.size();
	}

private:
	std::span<std::byte> m_buffer{};
	size_t m_offset = 0;
};


