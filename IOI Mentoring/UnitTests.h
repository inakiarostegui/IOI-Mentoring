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
bool unit_test_alloc_linear_init();
bool unit_test_alloc_linear_allocate_0();
bool unit_test_alloc_linear_allocate_1();
bool unit_test_alloc_linear_free_0();
bool unit_test_alloc_linear_init_clear();
bool unit_test_alloc_stack_init();
bool unit_test_alloc_stack_allocate_0();
bool unit_test_alloc_stack_allocate_1();
bool unit_test_alloc_stack_free_0();
bool unit_test_alloc_stack_free_1();
bool unit_test_alloc_stack_clear();
bool unit_test_alloc_stack_init_clear();
#pragma endregion

void run_unit_tests(std::vector<e_UnitTestTypes>&& test_types_to_run);