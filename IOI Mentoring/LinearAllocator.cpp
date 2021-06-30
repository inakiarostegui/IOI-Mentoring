/***************************************************************************//**
 * @filename LinearAllocator.cpp
 * @brief	 Contains the linear allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "LinearAllocator.h"

void LinearAllocator::Init(std::span<std::byte>&& memory_buffer)
{
	if (memory_buffer.size() == 0)
	{
		debug_print("ERROR [LinearAllocator.cpp, LinearAllocator, void Init(std::span<std::byte>&&)]: Buffer size cannot be zero.");
		return;
	}

	m_buffer = std::forward<std::span<std::byte>>(memory_buffer);

	Clear();
}

void* LinearAllocator::Allocate(size_t size_in_bytes, const size_t& alignment)
{
	if (size_in_bytes == 0u)
	{
		debug_print("ERROR [LinearAllocator.cpp, LinearAllocator, void Init(std::span<std::byte>)]: Allocation size cannot be zero.");
		return nullptr;
	}

	if (alignment != 0u)
		size_in_bytes += CalculatePadding(reinterpret_cast<uintptr_t>(m_buffer.data()) + m_offset + size_in_bytes, alignment);

	// If the requested size cannot be allocated then dont attempt
	if (m_offset + size_in_bytes > m_buffer.size())
	{
		debug_print("ERROR [LinearAllocator.cpp, LinearAllocator, void* Allocate(size_t, const size_t&)]: Allocation is too large for remaining buffer.");
		return nullptr;
	}

	// Update the offset by "moving" it by the given size
	m_offset += size_in_bytes;

	// Return pointer to allocated block
	return &m_buffer[m_offset - size_in_bytes];
}

void LinearAllocator::Clear()
{
	// Resets all data
	m_offset = 0;
}

void LinearAllocator::Free()
{
	// Linear allocators just clear the whole buffer
	Clear();
}