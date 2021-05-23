/***************************************************************************//**
 * @filename IAllocator.h
 * @brief	 Contains the allocator interface class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"

class IAllocator
{
public:
	virtual ~IAllocator() {};

	// Initialize the allocator
	virtual void Init(std::byte*& memory_buffer, const unsigned memory_buffer_length_in_bytes/*std::span<std::byte> memory_buffer*/) = 0;

	// Allocator a certain size in our allocator
	virtual void* Allocate(const unsigned size) = 0;

	// Free memory (at certain location if passed a ptr)
	virtual void Free(void* ptr) {};
	virtual void Free() {};

	// Clear all the memory
	virtual void Clear() = 0;

	// Prints information about the allocator, for debugging purposes
	virtual void PrintData(const bool print_contents) const = 0;
};