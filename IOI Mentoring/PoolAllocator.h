#pragma once
#include "pch.h"
#include "IAllocator.h"


class PoolAllocator : public IAllocator
{
public:
	void Init(std::byte*& memory_buffer, const unsigned memory_buffer_length_in_bytes/*std::span<std::byte>* memory_buffer*/)
	{
		if (m_buffer != nullptr)
			Clear();

		m_buffer = new std::byte[memory_buffer_length_in_bytes];
		memory_buffer = m_buffer;

		m_buffer_size = memory_buffer_length_in_bytes;
	}

	//should return nullptr if it can't/won't allocate
	void* Allocate(const unsigned size_in_bytes)
	{
		if (m_offset + size_in_bytes > m_buffer_size)
			return nullptr;

		m_offset += size_in_bytes;
		m_headers.push(size_in_bytes);

		return &m_buffer[m_offset - size_in_bytes];
	}

	//should do nothing if passed nullptr, undefined behavior (e.g. crash) if passed a pointer that wasn't previously allocated by the same allocator
	void Free(void* ptr)
	{
		if (!m_headers.empty())
		{
			m_offset -= m_headers.top();
			m_headers.pop();
		}
	}

	void Clear()
	{
		delete[] m_buffer;
		m_buffer = nullptr;
		m_offset = 0u;

		m_headers.clear();
	}

	~PoolAllocator()
	{
		Clear();
	}

	void PrintData()
	{
		if (m_buffer == nullptr)
		{
			std::cout << "Buffer Empty" << std::endl;
			return;
		}
		std::cout << "m_buffer_size: " << m_buffer_size << std::endl;
		std::cout << "m_offset: " << m_offset << std::endl;

		//for (unsigned i = 0u; i < m_buffer_size; i++)
		//	std::cout << std::to_integer<int>(m_buffer[i]) << " ";
		//std::cout << std::endl
	}

private:
	std::byte* m_buffer = nullptr;
	unsigned m_offset = 0u;
	mutable unsigned m_buffer_size = 0u;
	std::list<int> m_headers;
};





