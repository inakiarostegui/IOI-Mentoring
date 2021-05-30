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
	virtual ~IAllocator() {};

	virtual void Clear() = 0;

	virtual void Reset() = 0;

	// Prints information about the allocator, for debugging purposes
	virtual void PrintData(const bool print_contents) const = 0;
};