/***************************************************************************//**
 * @filename PoolAllocator.cpp
 * @brief	 Contains the pool allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "PoolAllocator.h"

std::byte** PoolAllocator::Init(const unsigned chunk_size_in_bytes, const unsigned chunks_amount)
{
	// If theres an existing buffer then reset it to recreate it
	if (m_buffer != nullptr)
		Reset();

	if (chunk_size_in_bytes < sizeof(PoolAllocationHeader))
	{
		std::cout << "ERROR [PoolAllocator.cpp, PoolAllocator, std::byte** Init(const unsigned, const unsigned)]: Chunk size was less than header size (std::byte*)." << std::endl;
		assert(0);
	}

	// Allocate requested memory
	m_buffer = static_cast<std::byte*>(malloc(chunk_size_in_bytes * static_cast<size_t>(chunks_amount)));
	assert(m_buffer != nullptr);

	m_chunk_size = chunk_size_in_bytes;
	m_chunks_amount = chunks_amount;

	m_free_list_head = m_buffer;
	
	// Write header info for each chunk
	for (unsigned i = 0u; i < chunks_amount - 1; i++)
		new (&m_buffer[i * chunk_size_in_bytes]) PoolAllocationHeader(&m_buffer[(i + 1) * chunk_size_in_bytes]);
	new (&m_buffer[(chunks_amount - 1) * chunk_size_in_bytes]) PoolAllocationHeader();

	return &m_buffer;
}

void* PoolAllocator::Allocate()
{
	// If theres no free chunks then cant allocate
	if (m_free_list_head == nullptr)
		return nullptr;

	// Remove the chunk from the container with the free chunks
	std::byte* free_list_head_temp = m_free_list_head;
	m_free_list_head = reinterpret_cast<PoolAllocationHeader*>(m_free_list_head)->m_free_list_next;

	// Return a ptr to the allocated memory
	return free_list_head_temp;
}

void PoolAllocator::Free(void* ptr)
{
	// If its already free then do nothing
	if (IsChunkFree(ptr))
		return;

	// Distance from ptr to buffer
	ptrdiff_t ptr_buffer_dist = (std::byte*)ptr - m_buffer;

	// Doesn't have to be sorted so we can just place the freed chunk at the start
	new (ptr) PoolAllocationHeader(m_free_list_head);
	m_free_list_head = static_cast<std::byte*>(ptr);
}

bool PoolAllocator::IsChunkFree(void* ptr) const
{
	if (!IsChunkPtrValid(ptr) || m_free_list_head == ptr)
		return true;
	if (m_free_list_head == nullptr)
		return false;

	// Check if the chunk is free by checking the headers of each free chunk
	std::byte* it = m_free_list_head;
	while (it != nullptr)
	{
		if (it == ptr)
			return true;
		it = reinterpret_cast<PoolAllocationHeader*>(it)->m_free_list_next;
	}
	return false;
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
	m_free_list_head = m_buffer;
	
	// Write header info for each chunk
	for (unsigned i = 0u; i < m_chunks_amount - 1; i++)
		new (&m_buffer[i * m_chunk_size]) PoolAllocationHeader(&m_buffer[(i + 1) * m_chunk_size]);
	new (&m_buffer[(m_chunks_amount - 1) * m_chunk_size]) PoolAllocationHeader();
}

void PoolAllocator::Reset()
{
	// Free allocated memory and reset all data
	free(m_buffer);
	m_buffer = nullptr;

	m_chunks_amount = 0u;
	m_chunk_size = 0u;

	m_free_list_head = nullptr;
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

	if (print_contents)
	{
		for (unsigned i = 0u; i < GetBufferSize(); i++)
			std::cout << std::to_integer<int>(m_buffer[i]) << " ";
		std::cout << std::endl;
	}
}