/***************************************************************************//**
 * @filename FreeListAllocator.cpp
 * @brief	 Contains the free list class function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "FreeListAllocator.h"

constexpr unsigned SIZE_ALLOC_HEADER = sizeof(FreeListAllocator::FreeListAllocHeader);
constexpr unsigned SIZE_FREE_HEADER = sizeof(FreeListAllocator::FreeListFreeHeader);

std::byte** FreeListAllocator::Init(const unsigned& memory_buffer_length_in_bytes)
{
	// If theres an existing buffer then reset it to recreate it
	if (m_buffer != nullptr)
		Reset();

	// Allocate requested memory
	m_buffer = static_cast<std::byte*>(malloc(memory_buffer_length_in_bytes));
	assert(m_buffer != nullptr);

	m_buffer_size = memory_buffer_length_in_bytes;

	// Safety check, buffer size cant be less than freeheader size
	if (memory_buffer_length_in_bytes < SIZE_FREE_HEADER)
	{
		debug_print("ERROR [FreeListAllocator.cpp, FreeListAllocator, std::byte** Init(const unsigned)]: Buffer size cannot be less than FreeHeader size.");
		assert(0);
	}

	// Initialize free chunks, only chunk we have is the entire buffer
	m_free_list_head = new (&m_buffer[0]) FreeListFreeHeader(memory_buffer_length_in_bytes - SIZE_ALLOC_HEADER);

	return &m_buffer;
}

// Allocates data of the given size inside the given chunk, size_in_bytes does not include headersize
void* FreeListAllocator::AllocateAtChunk(unsigned size_in_bytes, FreeListFreeHeader* prev_free_chunk)
{
	// If there is no previous free chunk then were trying to allocate at the head of the free list
	if (prev_free_chunk == nullptr)
	{
		FreeListFreeHeader* alloc_free_chunk = m_free_list_head;
		
		// If the size is exactly what we need then no need to create a new free chunk
		if (m_free_list_head->m_chunk_size == size_in_bytes)
			m_free_list_head = m_free_list_head->m_free_list_next;
		// If we cant fit a free chunk with the remaining memory, extend the allocated chunk by however much we have extra
		else if (m_free_list_head->m_chunk_size - size_in_bytes <= SIZE_FREE_HEADER)
		{
			size_in_bytes = m_free_list_head->m_chunk_size;
			m_free_list_head = m_free_list_head->m_free_list_next;
		}
		// Create a free chunk with the remaining free chunk we are not using from the allocation
		else
			m_free_list_head = new (reinterpret_cast<std::byte*>(m_free_list_head) + size_in_bytes + SIZE_ALLOC_HEADER) 
							   FreeListFreeHeader(m_free_list_head->m_chunk_size - size_in_bytes - SIZE_ALLOC_HEADER, m_free_list_head->m_free_list_next);
		
		// Pointer to allocated chunk, add the size of the alloc header as that is where the user can actually allocate memory
		return reinterpret_cast<std::byte*>(new (alloc_free_chunk) FreeListAllocHeader(size_in_bytes)) + SIZE_ALLOC_HEADER;
	}
	// Otherwise, there is a previous free chunk so were allocating at its next in the list
	else
	{
		FreeListFreeHeader* alloc_free_chunk = prev_free_chunk->m_free_list_next;

		if (prev_free_chunk->m_chunk_size == size_in_bytes)
			prev_free_chunk->m_free_list_next = prev_free_chunk->m_free_list_next->m_free_list_next;
		else if (prev_free_chunk->m_chunk_size - size_in_bytes <= SIZE_FREE_HEADER)
		{
			size_in_bytes = prev_free_chunk->m_chunk_size;
			prev_free_chunk->m_free_list_next = prev_free_chunk->m_free_list_next->m_free_list_next;
		}
		else
		{
			// Reduce the size of the chunk were allocating at
			alloc_free_chunk->m_chunk_size -= size_in_bytes + SIZE_ALLOC_HEADER;
			prev_free_chunk->m_free_list_next = new (reinterpret_cast<std::byte*>(alloc_free_chunk) + size_in_bytes + SIZE_ALLOC_HEADER)
												FreeListFreeHeader(*alloc_free_chunk);
		}

		// Pointer to allocated chunk
		return reinterpret_cast<std::byte*>(new (alloc_free_chunk) FreeListAllocHeader(size_in_bytes)) + SIZE_ALLOC_HEADER;
	}
}

void* FreeListAllocator::Allocate(const unsigned& size_in_bytes)
{
	if (m_free_list_head == nullptr || size_in_bytes == 0u)
		return nullptr;

	// Having an allocation size + alloc header being less than the free header size may lead to having chunks of memory that cannot be labelled as free
	if (size_in_bytes + SIZE_ALLOC_HEADER < SIZE_FREE_HEADER)
	{
		debug_print("WARNING [FreeListAllocator.cpp, FreeListAllocator, void* Allocate(const unsigned&)]: Allocation size cannot be less than 12 bytes (SIZE_FREE_HEADER - SIZE_ALLOC_HEADER).");
		return (this->*m_alloc_fns[static_cast<int>(m_alloc_type)])(SIZE_FREE_HEADER);
	}

	// Call the allocation function depending on what allocation type we have set
	return (this->*m_alloc_fns[static_cast<int>(m_alloc_type)])(size_in_bytes);
}

void* FreeListAllocator::AllocateBestFit(const unsigned& size_in_bytes)
{
	// First: did we find a valid chunk?	Second: ptr to previous free chunk
	std::pair<FreeListFreeHeader*, bool> best_free_chunk(nullptr, false);

	FreeListFreeHeader* prev_it = nullptr;
	FreeListFreeHeader* it = m_free_list_head;
	while (it != nullptr)
	{
		// If the free chunk size is larger than the size we want to allocate then go ahead
		if (it->m_chunk_size >= size_in_bytes)
		{
			// If we havent found a valid chunk yet then go ahead and use the current it value
			// Otherwise, check if it is better than the currently stored chunk (also if the value is nullptr then that means we have to use the head of the list)
			if (!best_free_chunk.second ?
					true :
					it->m_chunk_size <= (best_free_chunk.first == nullptr ?
											m_free_list_head->m_chunk_size :
											best_free_chunk.first->m_chunk_size))
			{
				best_free_chunk = std::make_pair(prev_it, true);
			}
		}

		prev_it = it;
		it = it->m_free_list_next;
	}

	// If the bool is true then we have set data, return it
	return best_free_chunk.second ? AllocateAtChunk(size_in_bytes, best_free_chunk.first) : nullptr;
}

void* FreeListAllocator::AllocateFirstFit(const unsigned& size_in_bytes)
{
	// LINEAR SEARCH for the first free chunk that can fit our bytes
	FreeListFreeHeader* prev_it = nullptr;
	FreeListFreeHeader* it = m_free_list_head;
	while (it != nullptr)
	{
		if (it->m_chunk_size >= size_in_bytes)
			return AllocateAtChunk(size_in_bytes, prev_it);

		prev_it = it;
		it = it->m_free_list_next;
	}
	
	return nullptr;
}

bool FreeListAllocator::AreChunksAdjacent(FreeListFreeHeader* chunk_0, FreeListFreeHeader* chunk_1) const
{
	// Safety check
	if (chunk_0 == nullptr || chunk_1 == nullptr)
		return false;

	// If the end of one chunk is the start of the other then they are adjacent
	return reinterpret_cast<std::byte*>(chunk_0) + (chunk_0->m_chunk_size + SIZE_ALLOC_HEADER) == reinterpret_cast<std::byte*>(chunk_1) ||
		   reinterpret_cast<std::byte*>(chunk_1) + (chunk_1->m_chunk_size + SIZE_ALLOC_HEADER) == reinterpret_cast<std::byte*>(chunk_0);
}

void FreeListAllocator::Free(void* ptr)
{
	// Move the ptr to the start of the alloc chunk, make it point to the header
	ptr = static_cast<std::byte*>(ptr) - SIZE_ALLOC_HEADER;

	// Check the ptr's validity and find the previous free chunk if possible
	FreeListFreeHeader* prev_free_chunk;
	if (!IsChunkPtrValid(ptr, &prev_free_chunk))
		return;

	// Depending on our previous chunk, get our next free chunk
	FreeListFreeHeader* next_free_chunk = prev_free_chunk == nullptr ? m_free_list_head : prev_free_chunk->m_free_list_next;

	// In the following line we write at the same location where we need to read, so the data is garbage, store a temporary variable for the size of the chunk
	const unsigned chunk_size = reinterpret_cast<FreeListAllocHeader*>(ptr)->m_chunk_size;
	// Write our new free chunk into the buffer at the given position
	FreeListFreeHeader* new_free_chunk = new (ptr) FreeListFreeHeader(chunk_size, next_free_chunk);

	// If the free list is empty make our new chunk the head
	if (m_free_list_head == nullptr)
	{
		m_free_list_head = new_free_chunk;
		return;
	}

	// Check adjacency backwards
	if (AreChunksAdjacent(prev_free_chunk, new_free_chunk))
	{
		// Increase the size of the previous chunk accordingly
		prev_free_chunk->m_chunk_size += new_free_chunk->m_chunk_size + SIZE_ALLOC_HEADER;	// This is how much space an allocated chunk takes
		new_free_chunk = prev_free_chunk;
	}
	// Unless our free chunk is the head, update the list pointers
	else if (prev_free_chunk != nullptr)
		prev_free_chunk->m_free_list_next = new_free_chunk;

	// Check adjacency forward
	if (AreChunksAdjacent(new_free_chunk, next_free_chunk))
	{
		// Increase the size of our new/updated free chunk accordingly and update the list pointers
		new_free_chunk->m_chunk_size += next_free_chunk->m_chunk_size + SIZE_ALLOC_HEADER;
		new_free_chunk->m_free_list_next = next_free_chunk->m_free_list_next;
	}
	else
	{
		// If the chunks are not adjancent update the list pointers
		new_free_chunk->m_free_list_next = next_free_chunk;

		// If our free chunk is the head then set it as so
		if (prev_free_chunk == nullptr)
			m_free_list_head = new_free_chunk;
	}
}

// Check if the ptr points outside the buffer, doesnt point to a chunk or is nullptr
bool FreeListAllocator::IsChunkPtrValid(void* ptr, FreeListFreeHeader** prev_free_chunk)
{
	// Safety check
	if (ptr == nullptr)
		return false;

	// Check if the ptr is pointing somewhere inside the buffer
	if (ptr < m_buffer || ptr > m_buffer + m_buffer_size)
	{
		debug_print("WARNING [FreeListAllocator.cpp, FreeListAllocator, bool IsChunkPtrValid(void*, FreeListFreeHeader**)]: Ptr to deallocate was not in buffer.");
		return false;
	}

	if (ptr == m_free_list_head)
		return false;

	// NOTE: The way we determine the validity of the ptr is with the free chunks, so if we have no free chunks, or the ptr is behind the head of the free list, 
	// we cannot determine its validity. Therefore this also means that we cannot check if the ptr is aligned with an alloc chunk.

	if (ptr < m_free_list_head)
	{
		*prev_free_chunk = nullptr;

		debug_print("WARNING [FreeListAllocator.cpp, FreeListAllocator, bool IsChunkPtrValid(void*, FreeListFreeHeader**)]: Ptr was infront of free list head. Cannot determine validity.");
		return true;
	}

	FreeListFreeHeader* it = m_free_list_head;
	while (it != nullptr)
	{
		// If the ptr is contained within a free chunk then its not valid
		if (ptr >= it && ptr < reinterpret_cast<std::byte*>(it) + it->m_chunk_size + SIZE_ALLOC_HEADER)
			return false;

		// If the ptr is between two free chunks then it is valid
		if (ptr >= reinterpret_cast<std::byte*>(it) + it->m_chunk_size + SIZE_ALLOC_HEADER &&
			(it->m_free_list_next == nullptr ?
				true :
				ptr < it->m_free_list_next))
		{
			*prev_free_chunk = it;
			return true;
		}

		it = it->m_free_list_next;
	}

	// With the information we have, this is an assumption, if the ptr the user passes is isnt aligned with a alloc chunk and this statement is true, then we have an issue
	if (m_free_list_head == nullptr)
	{
		*prev_free_chunk = nullptr;

		debug_print("WARNING [FreeListAllocator.cpp, FreeListAllocator, bool IsChunkPtrValid(void*, FreeListFreeHeader**)]: Cannot check ptr validity without free chunks.");
		return true;
	}

	return false;
}

void FreeListAllocator::Clear()
{
	// Resets all data and prepares for reuse without changing allocated memory
	m_free_list_head = new (&m_buffer[0]) FreeListFreeHeader();
}

void FreeListAllocator::Reset()
{
	// Free allocated memory and reset all data
	delete[] m_buffer;
	m_buffer = nullptr;
	m_free_list_head = nullptr;

	m_buffer_size = 0u;
}

void FreeListAllocator::PrintData(const bool print_contents) const
{
	if (m_buffer == nullptr)
	{
		std::cout << "Buffer Empty" << std::endl;
		return;
	}

	std::cout << "m_buffer_size: " << m_buffer_size << std::endl;

	if (print_contents)
	{
		for (unsigned i = 0u; i < GetBufferSize(); i++)
			std::cout << std::to_integer<int>(m_buffer[i]) << " ";
		std::cout << std::endl;
	}

	std::cout << "--------------------------------" << std::endl;
}