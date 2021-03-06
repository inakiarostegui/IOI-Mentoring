/***************************************************************************//**
 * @filename UnitTests.cpp
 * @brief	 Contains the unit test container implementation and run test
 *           function implentation.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"

const std::string PASS = "PASS";
const std::string FAIL = ">>>>>>>>>>>>>>>>>>>>>>>>> FAIL <<<<<<<<<<<<<<<<<<<<<<<<";

const std::array<std::string, 6> UT_TITLES = { "VECTORS", "MOVE SEMANTICS", "LINEAR ALLOCATOR", "STACK ALLOCATOR", "POOL ALLOCATOR", "FREE LIST ALLOCATOR", };

using namespace UT;
using namespace MoveSemantics;
using namespace Allocator;
using namespace Vectors;

// Contains the unit tests by categories
std::unordered_map<e_UTTypes, std::vector<UnitTest>> unit_tests =
{
    std::make_pair
    (
        e_UTTypes::e_vectors,
        std::vector<UnitTest>
        {
            UnitTest{"RESERVE 0",               &reserve_0},
            UnitTest{"RESERVE 1",               &reserve_1},
            UnitTest{"PUSH BACK 0",             &push_back_0},
            UnitTest{"PUSH BACK 1",             &push_back_1},
            UnitTest{"PUSH BACK 2",             &push_back_2},
            UnitTest{"EMPLACE BACK 0",          &emplace_back_0},
            UnitTest{"EMPLACE BACK 1",          &emplace_back_1},
            UnitTest{"EMPLACE BACK 2",          &emplace_back_2},
            UnitTest{"POP BACK 0",              &pop_back_0},
            UnitTest{"POP BACK 1",              &pop_back_1},
            UnitTest{"CLEAR",                   &clear},
            UnitTest{"SUBSCRIPT OPERATOR 0",    &subscript_0},
            UnitTest{"PRODUCTION",              &prod},
        }
    ),
    std::make_pair
    (
        e_UTTypes::e_move_semantics,
        std::vector<UnitTest>
        {
            UnitTest{"MOVE ASSIGNMENT",     &move_assignment},
            UnitTest{"MOVE CONSTRUCTOR 0",  &move_semantics_0},
            UnitTest{"MOVE CONSTRUCTOR 1",  &move_semantics_1},

            UnitTest{"PUSH LVALUE 0",       &push_lvalue_0},
            UnitTest{"PUSH LVALUE 1",       &push_lvalue_1},
            UnitTest{"PUSH RVALUE 0",       &push_rvalue_0},
            UnitTest{"PUSH RVALUE 1",       &push_rvalue_1},
            UnitTest{"PUSH RVALUE 2",       &push_rvalue_2},

            UnitTest{"EMPLACE LVALUE 0",    &emplace_lvalue_0},
            UnitTest{"EMPLACE LVALUE 1",    &emplace_lvalue_1},
            UnitTest{"EMPLACE RVALUE 0",    &emplace_rvalue_0},
            UnitTest{"EMPLACE RVALUE 1",    &emplace_rvalue_1},
            UnitTest{"EMPLACE RVALUE 2",    &emplace_rvalue_2},
        }
    ),
    std::make_pair
    (
        e_UTTypes::e_alloc_linear,
        std::vector<UnitTest>
        {
            UnitTest{"INIT",         &linear_init        },
            UnitTest{"ALLOCATE 0",   &linear_allocate_0  },
            UnitTest{"ALLOCATE 1",   &linear_allocate_1  },
            UnitTest{"FREE 0",       &linear_free_0      },
            UnitTest{"PRODUCTION",   &linear_prod        },
        }
    ),
    std::make_pair
    (
        e_UTTypes::e_alloc_stack,
        std::vector<UnitTest>
        {
            UnitTest{"INIT",          &stack_init         },
            UnitTest{"ALLOCATE 0",    &stack_allocate_0   },
            UnitTest{"ALLOCATE 1",    &stack_allocate_1   },
            UnitTest{"FREE 0",        &stack_free_0       },
            UnitTest{"FREE 1",        &stack_free_1       },
            UnitTest{"CLEAR",         &stack_clear        },
            UnitTest{"PRODUCTION",    &stack_prod         },
        }
    ),
    std::make_pair
    (
        e_UTTypes::e_alloc_pool,
        std::vector<UnitTest>
        {
            UnitTest{"INIT",           &pool_init           },
            UnitTest{"ALLOCATE 0",     &pool_allocate_0     },
            UnitTest{"ALLOCATE 1",     &pool_allocate_1     },
            UnitTest{"FREE 0",         &pool_free_0         },
            UnitTest{"FREE 1",         &pool_free_1         },
            UnitTest{"FREE 1",         &pool_free_2         },
            UnitTest{"CLEAR",          &pool_clear          },
            UnitTest{"PRODUCTION",     &pool_prod           },
        }
    ),
    std::make_pair
    (
        e_UTTypes::e_alloc_freelist,
        std::vector<UnitTest>
        {
            UnitTest{"INIT",                    &freelist_init                  },
            UnitTest{"ALLOCATE FIRST FIT 0",    &freelist_allocate_firstfit_0   },
            UnitTest{"ALLOCATE FIRST FIT 1",    &freelist_allocate_firstfit_1   },
            UnitTest{"ALLOCATE FIRST FIT 2",    &freelist_allocate_firstfit_2   },
            UnitTest{"ALLOCATE FIRST FIT 3",    &freelist_allocate_firstfit_3   },
            UnitTest{"ALLOCATE BEST FIT 0",     &freelist_allocate_bestfit_0    },
            UnitTest{"ALLOCATE BEST FIT 1",     &freelist_allocate_bestfit_1    },
            UnitTest{"ALLOCATE BEST FIT 2",     &freelist_allocate_bestfit_2    },
            UnitTest{"FREE 0",                  &freelist_free_0				},
            UnitTest{"FREE 1",                  &freelist_free_1				},
            UnitTest{"FREE 2",                  &freelist_free_2			    },
            UnitTest{"CLEAR",                   &freelist_clear                 },
            UnitTest{"PRODUCTION",              &freelist_prod                  },
        }
    ),
};

void UT::RunUnitTests(std::vector<UT::e_UTTypes>&& test_types_to_run)
{
    bool overall_result = true;
    bool temp_result = true;
    for (unsigned i = 0u; i < test_types_to_run.size(); i++)
    {
        std::cout << "--------------" + UT_TITLES[static_cast<int>(test_types_to_run[i])] + "--------------" << std::endl;

        for (unsigned j = 0u; j < unit_tests[test_types_to_run[i]].size(); j++)
        {
            temp_result = unit_tests[test_types_to_run[i]][j].m_test_fn();
            overall_result = overall_result && temp_result;

            std::cout << unit_tests[test_types_to_run[i]][j].m_test_name << ": " << std::endl;
            std::cout << "RESULT: " << (temp_result ? PASS : FAIL);
            std::cout << std::endl;
            std::cout << "-------------------------" << std::endl;
        }
    }
    std::cout << "-------------- OVERALL RESULT: " << (overall_result ? PASS : FAIL) << "--------------" << std::endl;
    std::cout << "*Certain tests are meant to give errors/warnings." << std::endl;

}

// TODO: VECTOR UT

