/***************************************************************************//**
 * @filename PoolAllocator.cpp
 * @brief	 Contains the pool allocator class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "PoolAllocator.h"

void PoolAllocator::Init(std::span<std::byte>&& memory_buffer, const unsigned& chunk_size_in_bytes)
{
	if (memory_buffer.size() == 0)
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, void Init(std::span<std::byte>&&, const unsigned&)]: Buffer size cannot be zero.");
		return;
	}

	if (chunk_size_in_bytes < sizeof(PoolAllocationHeader))
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, void Init(std::span<std::byte>&&, const unsigned&)]: Chunk size was less than header size (std::byte*).");
		return;
	}

	if (memory_buffer.size() % chunk_size_in_bytes != 0)
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, void Init(std::span<std::byte>&&, const unsigned&)]: Chunk size was not a multiple of buffer size.");
		return;
	}

	m_buffer = std::forward<std::span<std::byte>>(memory_buffer);
	m_chunk_size = chunk_size_in_bytes;

	Clear();
}

void* PoolAllocator::Allocate()
{
	// If theres no free chunks then cant allocate
	if (m_free_list_head == nullptr)
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, void* Allocate()]: No free chunks to allocate into.");
		return nullptr;
	}

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
	ptrdiff_t ptr_buffer_dist = (std::byte*)ptr - m_buffer.data();

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
	if (ptr < m_buffer.data() || ptr > m_buffer.data() + GetBufferSize())
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, bool IsChunkFree(void*)]: Ptr to deallocate was not in buffer.");
		return false;
	}

	const ptrdiff_t ptr_buffer_dist = (std::byte*)ptr - m_buffer.data();
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
	m_free_list_head = m_buffer.data();
	
	// Write header info for each chunk
	for (unsigned i = 0u; i < m_buffer.size() / m_chunk_size - 1; i++)
		new (&m_buffer[i * m_chunk_size]) PoolAllocationHeader(&m_buffer[(i + 1) * m_chunk_size]);
	new (&m_buffer[(m_buffer.size() / m_chunk_size - 1) * m_chunk_size]) PoolAllocationHeader();
}