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
	m_buffer = new std::byte[memory_buffer_length_in_bytes];

	m_buffer_size = memory_buffer_length_in_bytes;

	return &m_buffer;
}

void* StackAllocator::Allocate(const unsigned size_in_bytes)
{
	// If the requested size cannot be allocated then dont attempt
	if (m_offset + size_in_bytes > m_buffer_size)
		return nullptr;

	// Update the offset and headers stack for the new block of memory
	m_offset += size_in_bytes;
	m_headers.push(size_in_bytes);

	// Return pointer to allocated block
	return &m_buffer[m_offset - size_in_bytes];
}

void StackAllocator::Free()
{
	// Decrease offset by the size of the last allocated block in order to "free" it, then remove it from header stack
	if (!m_headers.empty())
	{
		m_offset -= m_headers.top();
		m_headers.pop();
	}
}

void StackAllocator::Clear()
{
	// Resets all data
	m_offset = 0u;
	
	while (!m_headers.empty())
		m_headers.pop();
}

void StackAllocator::Reset()
{
	// Free the allocated memory and reset all other data
	delete[] m_buffer;
	m_buffer = nullptr;

	m_offset = 0u;

	while (!m_headers.empty())
		m_headers.pop();
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

	if (!m_headers.empty())
		std::cout << "m_headers.size(): " << m_headers.size() << std::endl;

	if (print_contents)
	{
		for (unsigned i = 0u; i < m_buffer_size; i++)
			std::cout << std::to_integer<int>(m_buffer[i]) << " ";
		std::cout << std::endl;
	}
}