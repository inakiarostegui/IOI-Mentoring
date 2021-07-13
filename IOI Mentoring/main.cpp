/***************************************************************************//**
 * @filename main.cpp
 * @brief	 Main.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"	
#include "UnitTests.h"

using namespace UT;

int main(void)
{
	UT::RunUnitTests();
	//UT::RunUnitTests({ e_UTTypes::e_vectors });
	//UT::RunUnitTests({ e_UTTypes::e_move_semantics });
	//UT::RunUnitTests({ e_UTTypes::e_alloc_linear, e_UTTypes::e_alloc_stack, e_UTTypes::e_alloc_pool, e_UTTypes::e_alloc_freelist});

	return 0; 
}