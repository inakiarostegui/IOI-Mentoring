/***************************************************************************//**
 * @filename LinearAllocator.h
 * @brief	 Contains the linear allocator class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "IAllocator.h"

class LinearAllocator : public IAllocator
{
public:
	~LinearAllocator()
	{
		Free();
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

private:
	std::byte* m_buffer = nullptr;
	unsigned m_offset = 0u;
	mutable unsigned m_buffer_size = 0u;
}; 
