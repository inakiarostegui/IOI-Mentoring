/***************************************************************************//**
 * @filename FreeListAllocator.cpp
 * @brief	 Contains the free list class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "FreeListAllocator.h"

std::byte** FreeListAllocator::Init(const unsigned memory_buffer_length_in_bytes)
{
	// If theres an existing buffer then reset it to recreate it
	if (m_buffer != nullptr)
		Reset();

	// Allocate requested memory
	m_buffer = new std::byte[memory_buffer_length_in_bytes];

	m_buffer_size = memory_buffer_length_in_bytes;

	// Initialize free chunks, only chunk we have is the entire buffer
	m_free_chunks.push_front(MemoryChunk(0u, m_buffer_size));

	return &m_buffer;
}

void FreeListAllocator::InsertSortFreeChunk(MemoryChunk&& free_chunk, MemoryChunkListIterator* free_chunk_it)
{
	if (m_free_chunks.empty())
	{
		m_free_chunks.push_front(free_chunk);
		*free_chunk_it = m_free_chunks.begin();
		return;
	}

	// Binary search for the correct position
	MemoryChunkListIterator it = std::lower_bound(m_free_chunks.begin(), m_free_chunks.end(), free_chunk.m_dist_to_buffer,
												 [](const MemoryChunk& memory_chunk_0, unsigned dist)
												 {
												 	return memory_chunk_0.m_dist_to_buffer < dist;
												 });
	// LINEAR SEARCH
	/*for (MemoryChunkListIterator it = m_free_chunks.begin(); it != m_free_chunks.end(); it++)
	{
		if (it->m_dist_to_buffer > free_chunk.m_dist_to_buffer)
		{
			*free_chunk_it = m_free_chunks.insert(it, free_chunk);
			return;
		}
	}*/

	// Place the chunk in the correct order
	if (it != m_free_chunks.end())
	{
		*free_chunk_it = m_free_chunks.insert(it, free_chunk);
		return;
	}

	// We didnt find a chunk so place it at the back
	m_free_chunks.push_back(free_chunk);
	*free_chunk_it = --MemoryChunkListIterator(m_free_chunks.end());
}

// Inserts the given chunk into the sorted list of free chunks and concatenates adjacent chunks
MemoryChunk* FreeListAllocator::InsertFreeChunk(MemoryChunk&& free_chunk)
{
	MemoryChunkListIterator free_chunk_it;
	// Insert the chunk into the sorted list
	InsertSortFreeChunk(std::forward<MemoryChunk>(free_chunk), &free_chunk_it);

	// If its the head then no need to check adjacency with other chunks
	if (m_free_chunks.size() == 1)
		return &(free_chunk_it)._Ptr->_Myval;

	MemoryChunkListIterator temp_it(free_chunk_it);
	
	// Concatenate if previous adjacent chunk, merge the two values into free_chunk_it
	if (free_chunk_it != m_free_chunks.begin() &&
		AreChunksAdjancent(free_chunk_it._Ptr->_Prev->_Myval, *free_chunk_it))
	{
		ConcatenateChunks(&free_chunk_it._Ptr->_Myval, std::move(--temp_it));
		temp_it = free_chunk_it;
	}

	// Concatenate if next adjancet chunk, merge the two values into free_chunk_it
	if (free_chunk_it != m_free_chunks.end() &&
		AreChunksAdjancent(*free_chunk_it, free_chunk_it._Ptr->_Next->_Myval))
	{
		ConcatenateChunks(&free_chunk_it._Ptr->_Myval, std::move(++temp_it));
	}

	return &(free_chunk_it)._Ptr->_Myval;
}

bool FreeListAllocator::AreChunksAdjancent(const MemoryChunk& chunk_0, const MemoryChunk& chunk_1) const
{
	return (chunk_0.m_dist_to_buffer == chunk_1.m_dist_to_buffer + chunk_1.m_size ||
			chunk_0.m_dist_to_buffer + chunk_0.m_size == chunk_1.m_dist_to_buffer);
}

void FreeListAllocator::ConcatenateChunks(MemoryChunk* modificable_chunk, MemoryChunkListIterator&& chunk_to_erase_it)
{
	// Modify the current chunk to adapt the chunk to erase
	modificable_chunk->m_size += chunk_to_erase_it._Ptr->_Myval.m_size;
	modificable_chunk->m_dist_to_buffer = modificable_chunk->m_dist_to_buffer < chunk_to_erase_it._Ptr->_Myval.m_dist_to_buffer ?
										  modificable_chunk->m_dist_to_buffer : chunk_to_erase_it._Ptr->_Myval.m_dist_to_buffer;
	
	m_free_chunks.erase(chunk_to_erase_it);
}

// Allocates data of the given size inside the given chunk
void* FreeListAllocator::AllocateAtChunk(const unsigned size_in_bytes, MemoryChunkListIterator&& free_chunk_it)
{
	// If it fills the chunk then simply erase it from the free list
	if (free_chunk_it._Ptr->_Myval.m_size - size_in_bytes == 0u)
	{
		m_alloc_chunks.push_back(free_chunk_it._Ptr->_Myval);
		m_free_chunks.erase(free_chunk_it);
	}
	// Otherwise, no need to concatinate as behind there would be none, and next would have already been concatinated
	else
	{
		m_alloc_chunks.push_back(MemoryChunk(free_chunk_it._Ptr->_Myval.m_dist_to_buffer, size_in_bytes));
		free_chunk_it._Ptr->_Myval.m_dist_to_buffer += size_in_bytes;
		free_chunk_it._Ptr->_Myval.m_size -= size_in_bytes;
	}

	// Pointer to allocated chunk
	return &m_buffer[m_alloc_chunks.back().m_dist_to_buffer];
}

void* FreeListAllocator::Allocate(const unsigned size_in_bytes)
{
	if (m_free_chunks.empty())
		return nullptr;

	// Call the allocation function depending on what allocation type we have set
	return (this->*m_alloc_fns[static_cast<int>(m_alloc_type)])(size_in_bytes);
}

void* FreeListAllocator::AllocateBestFit(const unsigned size_in_bytes)
{
	// first: did we find a valid chunk?	second: iterator to chunk
	std::pair<bool, MemoryChunkListIterator> best_free_chunk_it = std::make_pair(false, MemoryChunkListIterator());
		
	// LINEAR SEARCH because we need to search through the entire list anyways
	for (MemoryChunkListIterator it = m_free_chunks.begin(); it != m_free_chunks.end(); it++)
	{
		if (it->m_size >= size_in_bytes && 
			(!best_free_chunk_it.first || (best_free_chunk_it.first && it->m_size <= best_free_chunk_it.second->m_size)))
		{
			// Which is better? above or below?
			// if (it->m_size >= size_in_bytes)
			//		if (!best_free_chunk_it.first || (best_free_chunk_it.first && it->m_size <= best_free_chunk_it.second->m_size))
				best_free_chunk_it = std::make_pair(true, it);
		}
	}

	// If the bool is true then we have set data, return it
	return best_free_chunk_it.first ? AllocateAtChunk(size_in_bytes, std::move(best_free_chunk_it.second)) : nullptr;
}

void* FreeListAllocator::AllocateFirstFit(const unsigned size_in_bytes)
{
	// LINEAR SEARCH for the first free chunk that can fit our bytes
	for (MemoryChunkListIterator it = m_free_chunks.begin(); it != m_free_chunks.end(); it++)
	{
		if (it->m_size >= size_in_bytes)
			return AllocateAtChunk(size_in_bytes, std::move(it));
	}
	
	return nullptr;
}

void FreeListAllocator::Free(void* ptr)
{
	MemoryChunkVectorIterator alloc_chunk_it;
	if (!IsChunkPtrValid(ptr, &alloc_chunk_it))
		return;

	MemoryChunk new_free_chunk;
	FreeAllocatedChunk(std::move(alloc_chunk_it), &new_free_chunk);

	// Inserts the free chunk into the free chunk list
	InsertFreeChunk(std::move(new_free_chunk));
}

// Removes the given chunk from the allocated chunk and stores its data
void FreeListAllocator::FreeAllocatedChunk(MemoryChunkVectorIterator&& alloc_chunk_it, MemoryChunk* new_free_chunk)
{
	// We need still need the information of the chunk even after removal, so store it
	*new_free_chunk = std::move(*alloc_chunk_it._Ptr);
	
	// Erase the chunk from the allocated chunks, we will later put it on the free chunks
	m_alloc_chunks.erase(alloc_chunk_it);
}

// Check if the ptr points outside the buffer, doesnt point to a chunk or is nullptr
bool FreeListAllocator::IsChunkPtrValid(void* ptr, MemoryChunkVectorIterator* alloc_chunk_it)
{
	if (ptr == nullptr)
		return false;

	// Check if the ptr is pointing somewhere inside the buffer
	if (ptr < m_buffer || ptr > m_buffer + m_buffer_size)
	{
		debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, bool IsChunkFree(void*)]: Ptr to deallocate was not in buffer.");
		return false;
	}

	// Calculate the distance from the ptr to the buffer
	const ptrdiff_t ptr_buffer_dist = (std::byte*)ptr - m_buffer;
	// Find a chunk with the same distance from the buffer
	*alloc_chunk_it = std::find_if(m_alloc_chunks.begin(), m_alloc_chunks.end(), 
								  [&ptr_buffer_dist](const MemoryChunk& memory_chunk) 
								  { 
								  	return memory_chunk.m_dist_to_buffer == ptr_buffer_dist;
								  });
	
	// If there was a valid free chunk found
	if (*alloc_chunk_it != m_alloc_chunks.end())
		return true;

	debug_print("ERROR [PoolAllocator.cpp, PoolAllocator, bool IsChunkFree(void*)]: Ptr to deallocate was not aligned with chunks.");
	return false;
}

void FreeListAllocator::Clear()
{
	// Resets all data and prepares for reuse without changing allocated memory
	m_alloc_chunks.clear();

	m_free_chunks.clear();
	m_free_chunks.push_front(MemoryChunk(0u, m_buffer_size));
}

void FreeListAllocator::Reset()
{
	// Free allocated memory and reset all data
	delete[] m_buffer;
	m_buffer = nullptr;

	m_buffer_size = 0u;
	m_alloc_chunks.clear();
	m_free_chunks.clear();
}

void FreeListAllocator::PrintData(const bool print_contents) const
{
	if (m_buffer == nullptr)
	{
		std::cout << "Buffer Empty" << std::endl;
		return;
	}

	std::cout << "m_buffer_size: " << m_buffer_size << std::endl;

	std::cout << "m_alloc_chunks: " << m_alloc_chunks.size() << std::endl;
	for (std::vector<MemoryChunk>::const_iterator it = m_alloc_chunks.begin(); it != m_alloc_chunks.end(); it++)
		std::cout << *it << " " << std::endl;
	std::cout << "------------" << std::endl;

	std::cout << "m_free_chunks: " << m_free_chunks.size() << std::endl;
	for (std::list<MemoryChunk>::const_iterator it = m_free_chunks.begin(); it != m_free_chunks.end(); it++)
		std::cout << *it << " " << std::endl;
	std::cout << "------------" << std::endl;

	if (print_contents)
	{
		for (unsigned i = 0u; i < GetBufferSize(); i++)
			std::cout << std::to_integer<int>(m_buffer[i]) << " ";
		std::cout << std::endl;
	}

	std::cout << "--------------------------------" << std::endl;
}