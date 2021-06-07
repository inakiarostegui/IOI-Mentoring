/***************************************************************************//**
 * @filename StackAllocator.cpp
 * @brief	 Contains the stack allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "StackAllocator.h"

std::byte** StackAllocator::Init(const unsigned memory_buffer_length_in_bytes)
{
	// If theres an existing buffer then reset it to recreate it
	if (m_buffer != nullptr)
		Reset();

	// Allocate requested memory
	m_buffer = reinterpret_cast<std::byte*>(malloc(memory_buffer_length_in_bytes));
	assert(m_buffer != nullptr);

	m_buffer_size = memory_buffer_length_in_bytes;

	return &m_buffer;
}

void* StackAllocator::Allocate(const unsigned size_in_bytes, const unsigned alignment)
{
	if (size_in_bytes == 0u)
		return nullptr;

	unsigned alloc_size = size_in_bytes + sizeof(StackAllocationFooter);
	if (alignment != 0u)
		alloc_size += CalculatePadding(reinterpret_cast<uintptr_t>(m_buffer) + m_offset + alloc_size, alignment);

	// If the requested size cannot be allocated then dont attempt
	if (m_offset + alloc_size > m_buffer_size)
		return nullptr;

	// Update the offset and headers stack for the new block of memory
	m_offset += alloc_size;
	
	new (&m_buffer[m_offset - sizeof(StackAllocationFooter)]) StackAllocationFooter(std::move(alloc_size));

	// Return pointer to allocated block
	return &m_buffer[m_offset - alloc_size];
}

void StackAllocator::Free()
{
	// Decrease offset by the size of the last allocated block (read the size from the footer) in order to "free" it
	if (m_offset - sizeof(StackAllocationFooter) >= 0u)
		m_offset -= reinterpret_cast<StackAllocationFooter*>(m_buffer + m_offset - sizeof(StackAllocationFooter))->m_alloc_size;	
}

void StackAllocator::Clear()
{
	m_offset = 0u;
}

void StackAllocator::Reset()
{
	// Free the allocated memory and reset all other data
	free(m_buffer);
	m_buffer = nullptr;

	m_offset = 0u;
}

void StackAllocator::PrintData(const bool print_contents) const
{
	if (m_buffer == nullptr)
	{
		std::cout << "Buffer Empty" << std::endl;
		return;
	}

	std::cout << "m_buffer_size: " << m_buffer_size << std::endl;
	std::cout << "m_offset: " << m_offset << std::endl;

	if (print_contents)
	{
		for (unsigned i = 0u; i < m_buffer_size; i++)
			std::cout << std::to_integer<int>(m_buffer[i]) << " ";
		std::cout << std::endl;
	}
}