/***************************************************************************//**
 * @filename StackAllocator.cpp
 * @brief	 Contains the stack allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "StackAllocator.h"

void StackAllocator::Init(std::span<std::byte>&& memory_buffer)
{
	if (memory_buffer.size() == 0)
	{
		debug_print("ERROR [StackAllocator.cpp, StackAllocator, void Init(std::span<std::byte>&&)]: Buffer size cannot be zero.");
		return;
	}

	m_buffer = std::forward<std::span<std::byte>>(memory_buffer);
	
	Clear();
}

void* StackAllocator::Allocate(size_t size_in_bytes, const size_t& alignment)
{
	if (size_in_bytes == 0)
	{
		debug_print("ERROR [StackAllocator.cpp, StackAllocator, void* Allocate(size_t, const size_t&)]: Allocation size cannot be zero.");
		return nullptr;
	}

	size_in_bytes += sizeof(StackAllocationFooter);
	if (alignment != 0)
		size_in_bytes += CalculatePadding(reinterpret_cast<uintptr_t>(m_buffer.data()) + m_offset + size_in_bytes, alignment);

	// If the requested size cannot be allocated then dont attempt
	if (m_offset + size_in_bytes > m_buffer.size())
	{
		debug_print("ERROR [StackAllocator.cpp, StackAllocator, void* Allocate(size_t, const size_t&)]: Allocation is too large for remaining buffer.");
		return nullptr;
	}

	// Update the offset and headers stack for the new block of memory
	m_offset += size_in_bytes;
	
	// I could get rid of a copy here by moving alloc_size, but it the code would look horrible when returning, not sure what is better...
	new (&m_buffer[m_offset - sizeof(StackAllocationFooter)]) StackAllocationFooter(size_in_bytes);

	// Return pointer to allocated block
	return &m_buffer[m_offset - size_in_bytes];
}

void StackAllocator::Free()
{
	// Decrease offset by the size of the last allocated block (read the size from the footer) in order to "free" it
	if (m_offset - sizeof(StackAllocationFooter) >= 0u)
		m_offset -= reinterpret_cast<StackAllocationFooter*>(m_buffer.data() + m_offset - sizeof(StackAllocationFooter))->m_alloc_size;	
}

void StackAllocator::Clear()
{
	m_offset = 0;
}
