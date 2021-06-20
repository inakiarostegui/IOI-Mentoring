/***************************************************************************//**
 * @filename AllocatorTestClass.cpp
 * @brief	 Contains the AllocatorTestClass class used by the allocator unit
 *			 tests.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"

class AllocatorTestClass
{
public:
	double y = 0.5;
	int x = 0;

	AllocatorTestClass() = default;

	AllocatorTestClass(AllocatorTestClass&& other) noexcept : x(other.x), y(other.y)
	{	}

	AllocatorTestClass(const double other_y, const int other_x) : x(other_x), y(other_y)
	{	}

	bool operator==(const AllocatorTestClass& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const AllocatorTestClass& other)
	{
		return x != other.x || y != other.y;
	}

	friend std::ostream& operator<<(std::ostream& out, const AllocatorTestClass& data)
	{
		return out << data.y << ' ' << data.x;
	}
};