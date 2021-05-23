/***************************************************************************//**
 * @filename StackAllocator.h
 * @brief	 Contains the stack allocator class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "IAllocator.h"


class StackAllocator : public IAllocator
{
public:
	~StackAllocator()
	{
		Clear();
	}

	void Init(std::byte*& memory_buffer, const unsigned memory_buffer_length_in_bytes/*std::span<std::byte>* memory_buffer*/);

	void* Allocate(const unsigned size_in_bytes);

	void Free();

	void Clear();

	void PrintData(const bool print_contents = false) const;

	unsigned GetOffset() const
	{
		return m_offset;
	}

	unsigned GetBufferSize() const
	{
		return m_buffer_size;
	}

	size_t GetAmountOfBlocks() const
	{
		return m_headers.size();
	}

private:
	std::byte* m_buffer = nullptr;
	unsigned m_offset = 0u;
	mutable unsigned m_buffer_size = 0u;

	// Stores the size of the blocks
	std::stack<int> m_headers;
};


