/***************************************************************************//**
 * @filename UnitTests.h
 * @brief	 Contains the unit test functions, container and run test function.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "UnitTest.h"

enum class e_UnitTestTypes { e_move_semantics, e_allocators };

#pragma region MOVE SEMANTICS & PERFECT FORWARDING UT
bool unit_test_move_assignment();
bool unit_test_move_semantics_0();
bool unit_test_move_semantics_1();
bool unit_test_push_lvalue_0();
bool unit_test_push_lvalue_1();
bool unit_test_push_rvalue_0();
bool unit_test_push_rvalue_1();
bool unit_test_push_rvalue_2();
bool unit_test_emplace_lvalue_0();
bool unit_test_emplace_lvalue_1();
bool unit_test_emplace_rvalue_0();
bool unit_test_emplace_rvalue_1();
bool unit_test_emplace_rvalue_2();
#pragma endregion

#pragma region ALLOCATOR UT
class AllocUTClass
{
public:
	double y = 0.5;
	int x = 0;

	AllocUTClass() = default;

	AllocUTClass(AllocUTClass&& other) noexcept : x(other.x), y(other.y) {	}
	
	AllocUTClass(const double other_y, const int other_x) : x(other_x), y(other_y) {	}

	bool operator==(const AllocUTClass& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const AllocUTClass& other)
	{
		return x != other.x || y != other.y;
	}

	friend std::ostream& operator<<(std::ostream& out, const AllocUTClass& data)
	{
		return out << data.y << ' ' << data.x;
	}
};

bool unit_test_alloc_linear_init();
bool unit_test_alloc_linear_allocate_0();
bool unit_test_alloc_linear_allocate_1();
bool unit_test_alloc_linear_free_0();
bool unit_test_alloc_linear_reset();
bool unit_test_alloc_linear_init_reset();
bool unit_test_alloc_linear_prod();
bool unit_test_alloc_stack_init();
bool unit_test_alloc_stack_allocate_0();
bool unit_test_alloc_stack_allocate_1();
bool unit_test_alloc_stack_free_0();
bool unit_test_alloc_stack_free_1();
bool unit_test_alloc_stack_clear();
bool unit_test_alloc_stack_reset();
bool unit_test_alloc_stack_init_reset();
bool unit_test_alloc_stack_prod();
bool unit_test_alloc_pool_init();
bool unit_test_alloc_pool_allocate_0();
bool unit_test_alloc_pool_allocate_1();
bool unit_test_alloc_pool_free_0();
bool unit_test_alloc_pool_free_1();
bool unit_test_alloc_pool_free_2();
bool unit_test_alloc_pool_clear();
bool unit_test_alloc_pool_reset();
bool unit_test_alloc_pool_init_reset();
bool unit_test_alloc_pool_prod();
#pragma endregion

void run_unit_tests(std::vector<e_UnitTestTypes>&& test_types_to_run);