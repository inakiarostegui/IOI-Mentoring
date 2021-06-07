/***************************************************************************//**
 * @filename VectorAllocator.cpp
 * @brief	 Contains the pool allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/
/*
#include "pch.h"
#include "VectorAllocator.h"

std::byte** PoolAllocator::Init(const unsigned chunk_size_in_bytes, const unsigned chunks_amount)
{
	// If theres an existing buffer then reset it to recreate it
	if (m_buffer != nullptr)
		Reset();

	// Allocate requested memory
	m_buffer = new std::byte[chunk_size_in_bytes * chunks_amount];

	m_chunk_size = chunk_size_in_bytes;
	m_chunks_amount = chunks_amount;

	// Initialize free chunk container with their respective distance from the buffer start
	m_free_chunk_buffer_dists.resize(chunks_amount);
	for (unsigned i = 0u; i < chunks_amount; i++)
		m_free_chunk_buffer_dists[i] = i * m_chunk_size;

	return &m_buffer;
}

void* PoolAllocator::Allocate()
{
	// If theres no free chunks then cant allocate
	if (m_free_chunk_buffer_dists.empty())
		return nullptr;

	// Remove the chunk from the container with the free chunks
	const unsigned free_chunk_buffer_dists = m_free_chunk_buffer_dists.back();
	m_free_chunk_buffer_dists.pop_back();

	// Return a ptr to the allocated memory
	return &m_buffer[free_chunk_buffer_dists];
}

void PoolAllocator::Free(void* ptr)
{
	// If its already free then do nothing
	if (IsChunkFree(ptr))
		return;

	// Distance from ptr to buffer
	ptrdiff_t ptr_buffer_dist = (std::byte*)ptr - m_buffer;

	// Add the free chunk to the container
	m_free_chunk_buffer_dists.push_back(static_cast<unsigned>(ptr_buffer_dist));
}

bool PoolAllocator::IsChunkFree(void* ptr) const
{
	if (!IsChunkPtrValid(ptr))
		return true;

	const ptrdiff_t ptr_buffer_dist = (std::byte*)ptr - m_buffer;
	// Check if the chunk is free (inside the free chunk container)
	std::vector<unsigned>::const_iterator it = std::find(m_free_chunk_buffer_dists.begin(), m_free_chunk_buffer_dists.end(), ptr_buffer_dist);
	if (it == m_free_chunk_buffer_dists.end())
		return false;

	return true;
}

bool PoolAllocator::IsChunkPtrValid(void* ptr) const
{
	if (ptr == nullptr)
		return false;

	// Check if the ptr is pointing somewhere inside the buffer
	if (ptr < m_buffer || ptr > m_buffer + GetBufferSize())
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, bool IsChunkFree(void*)]: Ptr to deallocate was not in buffer.");
		return false;
	}

	const ptrdiff_t ptr_buffer_dist = (std::byte*)ptr - m_buffer;
	// Check if the ptr points to the start of a chunk
	if (ptr_buffer_dist % m_chunk_size != 0)
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, bool IsChunkFree(void*)]: Ptr to deallocate was not aligned with chunks.");
		return false;
	}

	return true;
}

void PoolAllocator::Clear()
{
	// Resets all data and prepares for reuse without changing allocated memory
	m_free_chunk_buffer_dists.resize(m_chunks_amount);

	for (unsigned i = 0u; i < m_chunks_amount; i++)
		m_free_chunk_buffer_dists[i] = i * m_chunk_size;
}

void PoolAllocator::Reset()
{
	// Free allocated memory and reset all data
	delete[] m_buffer;
	m_buffer = nullptr;

	m_chunks_amount = 0u;
	m_chunk_size = 0u;

	m_free_chunk_buffer_dists.clear();
}

void PoolAllocator::PrintData(const bool print_contents) const
{
	if (m_buffer == nullptr)
	{
		std::cout << "Buffer Empty" << std::endl;
		return;
	}

	std::cout << "m_chunks_amount: " << m_chunks_amount << std::endl;
	std::cout << "m_chunk_size: " << m_chunk_size << std::endl;

	std::cout << "m_chunk_data: ";
	for (unsigned i = 0u; i < m_free_chunk_buffer_dists.size(); i++)
		std::cout << m_free_chunk_buffer_dists[i] / m_chunk_size << " ";
	std::cout << std::endl;

	if (print_contents)
	{
		for (unsigned i = 0u; i < GetBufferSize(); i++)
			std::cout << std::to_integer<int>(m_buffer[i]) << " ";
		std::cout << std::endl;
	}
}
*/