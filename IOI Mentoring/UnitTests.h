/***************************************************************************//**
 * @filename UnitTests.h
 * @brief	 Contains the unit test functions, container and run test function.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "UnitTest.h"

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

extern std::vector<UnitTest> unit_tests;

void run_unit_tests();