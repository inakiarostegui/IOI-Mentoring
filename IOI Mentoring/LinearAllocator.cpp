/***************************************************************************//**
 * @filename LinearAllocator.cpp
 * @brief	 Contains the linear allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "LinearAllocator.h"

std::byte** LinearAllocator::Init(const unsigned memory_buffer_length_in_bytes)
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

void* LinearAllocator::Allocate(const unsigned size_in_bytes, const unsigned alignment)
{
	if (size_in_bytes == 0u)
		return nullptr;

	unsigned padding_with_size = size_in_bytes;
	if (alignment != 0u)
		padding_with_size = CalculatePadding(reinterpret_cast<uintptr_t>(m_buffer) + m_offset + size_in_bytes, alignment) + size_in_bytes;

	// If the requested size cannot be allocated then dont attempt
	if (m_offset + padding_with_size > m_buffer_size)
		return nullptr;

	// Update the offset by "moving" it by the given size
	m_offset += padding_with_size;

	// Return pointer to allocated block
	return &m_buffer[m_offset - padding_with_size];
}

void LinearAllocator::Clear()
{
	// Resets all data
	m_offset = 0u;
}

void LinearAllocator::Reset()
{
	// Free the allocated memory and reset all other data
	free(m_buffer);
	m_buffer = nullptr;

	m_offset = 0u;
}

void LinearAllocator::Free()
{
	// Linear allocators just clear the whole buffer
	Clear();
}

void LinearAllocator::PrintData(const bool print_contents) const
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