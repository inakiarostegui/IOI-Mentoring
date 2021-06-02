/***************************************************************************//**
 * @filename FreeListMemoryChunk.h
 * @brief	 Contains the free list memory chunk class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"

class MemoryChunk
{
public:
	MemoryChunk() = default;

	MemoryChunk(const unsigned dist_to_buffer, const unsigned size) : m_dist_to_buffer(dist_to_buffer), m_size(size)
	{	}

	MemoryChunk(const MemoryChunk& other) noexcept
	{
		m_dist_to_buffer = other.m_dist_to_buffer;
		m_size = other.m_size;
	}

	MemoryChunk(MemoryChunk&& other) noexcept
	{
		m_dist_to_buffer = other.m_dist_to_buffer;
		m_size = other.m_size;

		other.m_dist_to_buffer = 0u;
		other.m_size = 0u;
	}

	MemoryChunk& operator=(MemoryChunk&& other) noexcept
	{
		if (this != &other)
		{
			m_dist_to_buffer = other.m_dist_to_buffer;
			m_size = other.m_size;

			other.m_dist_to_buffer = 0u;
			other.m_size = 0u;
		}
		return *this;
	}

	bool operator==(const MemoryChunk& other)
	{
		return other.m_dist_to_buffer == m_dist_to_buffer && other.m_size == m_size;
	}

	friend std::ostream& operator<<(std::ostream& out, const MemoryChunk& data)
	{
		return out << "m_dist_to_buffer: " << data.m_dist_to_buffer << "   " << "m_size: " << data.m_size;
	}

	unsigned m_dist_to_buffer = 0u;
	unsigned m_size = 0u;
};
