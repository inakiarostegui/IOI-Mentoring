/***************************************************************************//**
 * @filename IAllocator.h
 * @brief	 Contains the allocator interface class.
 * 
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"

class IAllocator
{
public:
	virtual void Clear() = 0;

	static uintptr_t CalculatePadding(const uintptr_t& alloc_address, const size_t& alignment)
	{
		return alloc_address % alignment;
	}
};