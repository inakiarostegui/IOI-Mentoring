/***************************************************************************//**
 * @filename LinearAllocator.cpp
 * @brief	 Contains the linear allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "LinearAllocator.h"

void LinearAllocator::Init(std::byte*& memory_buffer, const unsigned memory_buffer_length_in_bytes)
{
	// If theres an existing buffer then clear it to recreate it
	if (m_buffer != nullptr)
		Clear();

	// Allocate requested memory
	m_buffer = new std::byte[memory_buffer_length_in_bytes];
	// Send buffer pointer to user
	memory_buffer = m_buffer;

	m_buffer_size = memory_buffer_length_in_bytes;
}

void* LinearAllocator::Allocate(const unsigned size_in_bytes)
{
	// If the requested size cannot be allocated then dont attempt
	if (m_offset + size_in_bytes > m_buffer_size)
		return nullptr;

	// Update the offset by "moving" it by the given size
	m_offset += size_in_bytes;

	// Return pointer to allocated block
	return &m_buffer[m_offset - size_in_bytes];
}

void LinearAllocator::Clear()
{
	// Free the allocated memory and reset all other data
	delete[] m_buffer;
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