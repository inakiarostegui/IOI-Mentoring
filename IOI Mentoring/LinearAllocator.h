/***************************************************************************//**
 * @filename LinearAllocator.h
 * @brief	 Contains the linear allocator class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "IAllocator.h"

class LinearAllocator : public IAllocator
{
public:
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
	std::span<std::byte> m_buffer{};		// Would be void* if it were typed
	size_t m_offset = 0;
}; 
