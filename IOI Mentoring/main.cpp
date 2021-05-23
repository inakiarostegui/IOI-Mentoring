/***************************************************************************//**
 * @filename main.cpp
 * @brief	 Main.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"

int main(void)
{
	run_unit_tests({ e_UnitTestTypes::e_move_semantics, e_UnitTestTypes::e_allocators });

	return 0; 
}