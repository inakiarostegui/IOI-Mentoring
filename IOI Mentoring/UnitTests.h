/***************************************************************************//**
 * @filename UnitTests.h
 * @brief	 Contains the unit test function definitions, container and run test 
 *			 function definition.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "UnitTest.h"

namespace UT
{
	enum class e_UTTypes { e_vectors, e_move_semantics, e_alloc_linear, e_alloc_stack, e_alloc_pool, e_alloc_freelist };

	namespace MoveSemantics
	{
		bool move_assignment();
		bool move_semantics_0();
		bool move_semantics_1();				// R-value becomes L-value
		bool push_lvalue_0();
		bool push_lvalue_1();
		bool push_rvalue_0();
		bool push_rvalue_1();
		bool push_rvalue_2();
		bool emplace_lvalue_0();
		bool emplace_lvalue_1();
		bool emplace_rvalue_0();
		bool emplace_rvalue_1();
		bool emplace_rvalue_2();
	}

	namespace Allocator
	{
		bool linear_init();
		bool linear_allocate_0();				// Basic allocation
		bool linear_allocate_1();				// Invalid ptr allocation
		bool linear_free_0();					// Basic free
		bool linear_prod();

		bool stack_init();
		bool stack_allocate_0();				// Basic allocation
		bool stack_allocate_1();				// Invalid ptr allocation
		bool stack_free_0();					// Basic free
		bool stack_free_1();					// Empty free
		bool stack_clear();
		bool stack_prod();

		bool pool_init();
		bool pool_allocate_0();					// Basic allocation
		bool pool_allocate_1();					// Invalid ptr allocation
		bool pool_free_0();						// Basic free
		bool pool_free_1();						// Invalid ptr free
		bool pool_free_2();						// Invalid ptr free
		bool pool_clear();
		bool pool_prod();

		bool freelist_init();
		bool freelist_allocate_firstfit_0();	// Basic allocation 
		bool freelist_allocate_firstfit_1();	// Buffer filling allocation
		bool freelist_allocate_firstfit_2();	// Header fitting allocation 
		bool freelist_allocate_firstfit_3();	// First fit allocation
		bool freelist_allocate_bestfit_0();		// Basic allocation
		bool freelist_allocate_bestfit_1();		// Header fitting allocation
		bool freelist_allocate_bestfit_2();		// Best fit allocation
		bool freelist_free_0();					// Basic free (with list head and tail)
		bool freelist_free_1();					// Invalid ptr free
		bool freelist_free_2();					// Invalid ptr free
		bool freelist_clear();
		bool freelist_prod();					// Free chunk concatenation
	}

	namespace Vectors
	{
		bool reserve_0();						// Basic reserve
		bool reserve_1();						// Invalid reserve
		bool push_back_0();						// From empty
		bool push_back_1();						// Basic push_back
		bool push_back_2();						// Force grow
		bool emplace_back_0();					// From empty
		bool emplace_back_1();					// Basic push_back
		bool emplace_back_2();					// Force grow
		bool pop_back_0();						// Invalid pop_back
		bool pop_back_1();						// Basic pop_bac
		bool clear();							
		bool subscript_0();						
		bool prod();							// Pushing and popping
	}

	void RunUnitTests(std::vector<UT::e_UTTypes>&& test_types_to_run = { 
																		  e_UTTypes::e_vectors,
																		  e_UTTypes::e_move_semantics,
																		  e_UTTypes::e_alloc_linear,
																		  e_UTTypes::e_alloc_stack,
																		  e_UTTypes::e_alloc_pool,
																		  e_UTTypes::e_alloc_freelist,
																	   });
}