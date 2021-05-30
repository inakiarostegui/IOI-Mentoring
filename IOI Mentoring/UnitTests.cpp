/***************************************************************************//**
 * @filename UnitTests.cpp
 * @brief	 Contains the unit test functions, container and run test function.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "UnitTest.h"
#include "Vector.h"
#include "VectorTestClass.h"
#include "LinearAllocator.h"
#include "StackAllocator.h"
#include "PoolAllocator.h"

const std::string PASS = "PASS";
const std::string FAIL = "FAIL <<<<<<<<<<<<<<<<<<<<<<<<";

const std::array<std::string, 2> UT_TITLES = { "MOVE SEMANTICS", "ALLOCATORS" };

// Contains the unit tests by categories
std::unordered_map<e_UnitTestTypes, std::vector<UnitTest>> unit_tests =
{
    std::make_pair
    (
        e_UnitTestTypes::e_move_semantics, 
        std::vector<UnitTest>
        {
            UnitTest{"MOVE ASSIGNMENT",     &unit_test_move_assignment},
            UnitTest{"MOVE CONSTRUCTOR 0",  &unit_test_move_semantics_0},
            UnitTest{"MOVE CONSTRUCTOR 1",  &unit_test_move_semantics_1},
            UnitTest{"PUSH LVALUE 0",       &unit_test_push_lvalue_0},
            UnitTest{"PUSH LVALUE 1",       &unit_test_push_lvalue_1},
            UnitTest{"PUSH RVALUE 0",       &unit_test_push_rvalue_0},
            UnitTest{"PUSH RVALUE 1",       &unit_test_push_rvalue_1},
            UnitTest{"PUSH RVALUE 2",       &unit_test_push_rvalue_2},
            UnitTest{"EMPLACE LVALUE 0",    &unit_test_emplace_lvalue_0},
            UnitTest{"EMPLACE LVALUE 1",    &unit_test_emplace_lvalue_1},
            UnitTest{"EMPLACE RVALUE 0",    &unit_test_emplace_rvalue_0},
            UnitTest{"EMPLACE RVALUE 1",    &unit_test_emplace_rvalue_1},
            UnitTest{"EMPLACE RVALUE 2",    &unit_test_emplace_rvalue_2},
        }
    ),
    std::make_pair
    (
        e_UnitTestTypes::e_allocators,
        std::vector<UnitTest>
        {
            UnitTest{"LINEAR INIT",         &unit_test_alloc_linear_init        },
            UnitTest{"LINEAR ALLOCATE 0",   &unit_test_alloc_linear_allocate_0  },
            UnitTest{"LINEAR ALLOCATE 1",   &unit_test_alloc_linear_allocate_1  },
            UnitTest{"LINEAR FREE 0",       &unit_test_alloc_linear_free_0      },
            UnitTest{"LINEAR RESET",        &unit_test_alloc_linear_reset       },
            UnitTest{"LINEAR INIT CLEAR",   &unit_test_alloc_linear_init_reset  },
            UnitTest{"LINEAR PRODUCTION",   &unit_test_alloc_linear_prod        },

            UnitTest{"STACK INIT 0",        &unit_test_alloc_stack_init         },
            UnitTest{"STACK ALLOCATE 0",    &unit_test_alloc_stack_allocate_0   },
            UnitTest{"STACK ALLOCATE 1",    &unit_test_alloc_stack_allocate_1   },
            UnitTest{"STACK FREE 0",        &unit_test_alloc_stack_free_0       },
            UnitTest{"STACK FREE 1",        &unit_test_alloc_stack_free_1       },
            UnitTest{"STACK CLEAR",         &unit_test_alloc_stack_clear        },
            UnitTest{"STACK RESET",         &unit_test_alloc_stack_reset        },
            UnitTest{"STACK INIT CLEAR",    &unit_test_alloc_stack_init_reset   },
            UnitTest{"STACK PRODUCTION",    &unit_test_alloc_stack_prod   },

            UnitTest{"POOL INIT 0",        &unit_test_alloc_pool_init           },
            UnitTest{"POOL ALLOCATE 0",    &unit_test_alloc_pool_allocate_0     },
            UnitTest{"POOL ALLOCATE 1",    &unit_test_alloc_pool_allocate_1     },
            UnitTest{"POOL FREE 0",        &unit_test_alloc_pool_free_0         },
            UnitTest{"POOL FREE 1",        &unit_test_alloc_pool_free_1         },
            UnitTest{"POOL FREE 1",        &unit_test_alloc_pool_free_2         },
            UnitTest{"POOL CLEAR",         &unit_test_alloc_pool_clear          },
            UnitTest{"POOL RESET",         &unit_test_alloc_pool_reset          },
            UnitTest{"POOL INIT CLEAR",    &unit_test_alloc_pool_init_reset     },
            UnitTest{"POOL PRODUCTION",    &unit_test_alloc_pool_prod     },
        }
    ),

};

void run_unit_tests(std::vector<e_UnitTestTypes>&& test_types_to_run)
{
    for (unsigned i = 0u; i < test_types_to_run.size(); i++)
    {
        std::cout << "--------------" + UT_TITLES[static_cast<int>(test_types_to_run[i])] + "--------------" << std::endl;

        for (unsigned j = 0u; j < unit_tests[test_types_to_run[i]].size(); j++)
        {
            std::cout << unit_tests[test_types_to_run[i]][j].m_test_name << ": " << std::endl;
            std::cout << "RESULT: " << (unit_tests[test_types_to_run[i]][j].m_test_fn() ? PASS : FAIL);
            std::cout << std::endl;
            std::cout << "-------------------------" << std::endl;
        }
    }
}


#pragma region MOVE SEMANTICS & PERFECT FORWARDING UT
#pragma region VECTOR MOVE SEMANTICS UT
bool unit_test_move_assignment()
{
    Vector<int> vec_0{};
    Vector<int> vec_1{ 15 };

    // Using move assignment operator
    vec_0 = std::move(vec_1);

    return vec_0.capacity() == 15;
}

bool unit_test_move_semantics_0()
{
    Vector<int> vec_2{ 5 };

    // Using move semantics
    Vector<int> vec_3(std::move(vec_2));

    return vec_3.capacity() == 5;
}

bool unit_test_move_semantics_1()
{
    // Using move semantics
    BaseVector<int> bvec_0(10);

    return bvec_0.m_data.capacity() == 10;
}
#pragma endregion

#pragma region PERFECT FORWARDING UT
#pragma region PUSH UT
bool unit_test_push_rvalue_0()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;

    vec_last = vec.push_back(VectorTestClass(3, 4.0f));

    return *vec_last == VectorTestClass(3, 4.0f) && vec_last == &(vec[vec.size() - 1]);
}

bool unit_test_push_rvalue_1()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(11, 22.0f);

    vec_last = vec.push_back(std::move(vec_test));

    return *vec_last == VectorTestClass(11, 22.0f) && VectorTestClass(0, 0.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
}

bool unit_test_push_rvalue_2()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(3, -1.0f);

    vec_last = push_back(vec, std::move(vec_test));

    return *vec_last == VectorTestClass(3, -1.0f) && VectorTestClass(3, -1.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
}

bool unit_test_push_lvalue_0()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(9, 2.0f);

    vec_last = vec.push_back(vec_test);

    return *vec_last == VectorTestClass(9, 2.0f) && VectorTestClass(9, 2.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
}

bool unit_test_push_lvalue_1()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(3, -1.0f);

    vec_last = push_back(vec, vec_test);

    return *vec_last == VectorTestClass(3, -1.0f) && VectorTestClass(3, -1.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
}
#pragma endregion

#pragma region EMPLACE UT
bool unit_test_emplace_lvalue_0()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(9, -3.0f);

    vec_last = vec.emplace_back(vec_test);

    return *vec_last == VectorTestClass(9, -3.0f) && VectorTestClass(9, -3.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
}

bool unit_test_emplace_lvalue_1()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(13, 6.0f);

    vec_last = emplace_back(vec, vec_test);

    return *vec_last == VectorTestClass(13, 6.0f) && vec_test == VectorTestClass(13, 6.0f) && vec_last == &(vec[vec.size() - 1]);
}

bool unit_test_emplace_rvalue_0()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;

    vec_last = vec.emplace_back(7, 1.0f);

    return *vec_last == VectorTestClass(7, 1.0f) && vec_last == &(vec[vec.size() - 1]);
}

bool unit_test_emplace_rvalue_1()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(6, 12.0f);

    vec_last = vec.emplace_back(std::move(vec_test));

    return *vec_last == VectorTestClass(6, 12.0f) && vec_last == &(vec[vec.size() - 1]) && vec_test == VectorTestClass(0, 0.0f);
}

bool unit_test_emplace_rvalue_2()
{
    Vector<VectorTestClass> vec;
    VectorTestClass* vec_last = nullptr;
    VectorTestClass vec_test(6, 12.0f);

    vec_last = emplace_back(vec, std::move(vec_test));

    return *vec_last == VectorTestClass(6, 12.0f) && vec_last == &(vec[vec.size() - 1]) && vec_test == VectorTestClass(0, 0.0f);
}
#pragma endregion
#pragma endregion
#pragma endregion

#pragma region ALLOCATOR UT
#pragma region LINEAR ALLOCATOR UT
bool unit_test_alloc_linear_init()
{
    LinearAllocator la;
    std::byte** buffer = la.Init(48);

    return la.GetBufferSize() == 48u;
}

bool unit_test_alloc_linear_allocate_0()
{
    LinearAllocator la;
    std::byte** buffer = la.Init(48);
    la.Allocate(20);

    return la.GetOffset() == 20u;
}

bool unit_test_alloc_linear_allocate_1()
{
    LinearAllocator la;
    std::byte** buffer = la.Init(48);
    la.Allocate(20);
    la.Allocate(30);

    return la.GetOffset() == 20u;
}

bool unit_test_alloc_linear_free_0()
{
    LinearAllocator la;
    std::byte** buffer = la.Init(48);
    la.Allocate(20);
    la.Free();

    return la.GetOffset() == 0u && la.GetBufferSize() == 48u;
}

bool unit_test_alloc_linear_reset()
{
    LinearAllocator la;
    std::byte** buffer = la.Init(48);
    la.Allocate(20);
    la.Reset();

    return *buffer == nullptr;
}

bool unit_test_alloc_linear_init_reset()
{
    LinearAllocator la;
    std::byte** buffer = la.Init(48);
    la.Allocate(20);
    buffer = la.Init(36);

    return la.GetBufferSize() == 36u;
}

bool unit_test_alloc_linear_prod()
{
    LinearAllocator la;

    std::byte** buffer = la.Init(sizeof(AllocUTClass) * 8);
    
    AllocUTClass* data_0 = new (la.Allocate(sizeof(AllocUTClass))) AllocUTClass(1.2, 8);
    AllocUTClass* data_1 = new (la.Allocate(sizeof(AllocUTClass))) AllocUTClass(1.5, 10);

    la.Free();

    AllocUTClass* data_2 = new (la.Allocate(sizeof(AllocUTClass))) AllocUTClass(2.2, 5);
    AllocUTClass* data_3 = new (la.Allocate(sizeof(AllocUTClass))) AllocUTClass(2.6, 2);

    return la.GetOffset() == sizeof(AllocUTClass) * 2 && *data_2 == AllocUTClass(2.2, 5) && *data_3 == AllocUTClass(2.6, 2);
}
#pragma endregion

#pragma region STACK ALLOCATOR UT
bool unit_test_alloc_stack_init()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48);

    return sa.GetBufferSize() == 48u;
}

bool unit_test_alloc_stack_allocate_0()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48);
    sa.Allocate(20);

    return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
}

bool unit_test_alloc_stack_allocate_1()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48);
    sa.Allocate(20);
    sa.Allocate(30);

    return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
}

bool unit_test_alloc_stack_free_0()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48);
    sa.Allocate(20);
    sa.Allocate(20);
    sa.Free();

    return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
}

bool unit_test_alloc_stack_free_1()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48);
    sa.Free();

    return sa.GetBufferSize() == 48u;
}

bool unit_test_alloc_stack_clear()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48u);
    sa.Clear();

    return sa.GetOffset() == 0u && sa.GetAmountOfBlocks() == 0u && sa.GetBufferSize() == 48u;
}

bool unit_test_alloc_stack_reset()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48u);
    sa.Reset();

    return *buffer == nullptr;
}

bool unit_test_alloc_stack_init_reset()
{
    StackAllocator sa;
    std::byte** buffer = sa.Init(48);
    sa.Allocate(20);
    buffer = sa.Init(36);

    return sa.GetBufferSize() == 36u && sa.GetOffset() == 0u && sa.GetAmountOfBlocks() == 0;
}

bool unit_test_alloc_stack_prod()
{
    StackAllocator sa;

    std::byte** buffer = sa.Init(sizeof(AllocUTClass) * 8);

    AllocUTClass* data_0 = new (sa.Allocate(sizeof(AllocUTClass))) AllocUTClass(1.2, 8);
    AllocUTClass* data_1 = new (sa.Allocate(sizeof(AllocUTClass))) AllocUTClass(1.5, 10);

    sa.Free();

    AllocUTClass* data_2 = new (sa.Allocate(sizeof(AllocUTClass))) AllocUTClass(2.2, 5);
    AllocUTClass* data_3 = new (sa.Allocate(sizeof(AllocUTClass))) AllocUTClass(2.6, 2);

    return sa.GetAmountOfBlocks() == 3 && *data_0 == AllocUTClass(1.2, 8) && data_1 == data_2 && *data_2 == AllocUTClass(2.2, 5) && *data_3 == AllocUTClass(2.6, 2);
}
#pragma endregion

#pragma region POOL ALLOCATOR UT
bool unit_test_alloc_pool_init()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(8, 12);

    return pa.GetBufferSize() == 8u * 12u;
}

bool unit_test_alloc_pool_allocate_0()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);
    void* chunk_0 = pa.Allocate();
    void* chunk_1 = pa.Allocate();

    return pa.GetFreeChunks().size() == 10 && !pa.IsChunkFree(chunk_0) && !pa.IsChunkFree(chunk_1);
}

bool unit_test_alloc_pool_allocate_1()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);

    for (unsigned i = 0u; i < 12; i++)
        pa.Allocate();

    void* chunk_13 = pa.Allocate();

    return pa.GetFreeChunks().size() == 0 && chunk_13 == nullptr;
}

bool unit_test_alloc_pool_free_0()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);
    void* chunk_0 = pa.Allocate();
    void* chunk_1 = pa.Allocate();
    pa.Free(chunk_0);

    std::vector<unsigned> free_chunks = pa.GetFreeChunks();
    return free_chunks.size() == 11 && pa.IsChunkFree(chunk_0) && std::find(free_chunks.begin(), free_chunks.end(), 44) != free_chunks.end();
}

bool unit_test_alloc_pool_free_1()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);
    void* chunk_0 = pa.Allocate();
    int* temp_ptr = new int;
    pa.Free(reinterpret_cast<void*>(temp_ptr));

    return pa.GetFreeChunks().size() == 11;
}

bool unit_test_alloc_pool_free_2()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);
    char* chunk_0 = reinterpret_cast<char*>(pa.Allocate());
    chunk_0++;
    pa.Free(chunk_0);

    return pa.GetFreeChunks().size() == 11;
}

bool unit_test_alloc_pool_clear()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);
    void* chunk_0 = pa.Allocate();
    pa.Clear();

    return pa.GetFreeChunks().size() == 12;
}

bool unit_test_alloc_pool_reset()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);
    void* chunk_0 = pa.Allocate();
    pa.Reset();

    return *buffer == nullptr;
}

bool unit_test_alloc_pool_init_reset()
{
    PoolAllocator pa;
    std::byte** buffer = pa.Init(4, 12);
    pa.Allocate();
    buffer = pa.Init(3, 12);

    return pa.GetBufferSize() == 3 * 12 && pa.GetChunkSize() == 3u && pa.GetFreeChunks().size() == 12;
}

bool unit_test_alloc_pool_prod()
{
    PoolAllocator pa;

    std::byte** buffer = pa.Init(sizeof(AllocUTClass), 3);

    AllocUTClass* data_0 = new (pa.Allocate()) AllocUTClass(1.2, 8);
    AllocUTClass* data_1 = new (pa.Allocate()) AllocUTClass(1.5, 10);
    AllocUTClass* data_2 = new (pa.Allocate()) AllocUTClass(2.2, 5);

    pa.Free(data_0);
    pa.Free(data_2);

    AllocUTClass* data_3 = new (pa.Allocate()) AllocUTClass(2.6, 2);

    pa.Free(data_1);

    AllocUTClass* data_4 = new (pa.Allocate()) AllocUTClass(3.4, 3);
    AllocUTClass* data_5 = new (pa.Allocate()) AllocUTClass(0.2, 2);

    pa.Free(data_3);

    return pa.GetFreeChunks().size() == 1 && pa.IsChunkFree(data_3) 
        && *data_4 == AllocUTClass(3.4, 3) && *data_5 == AllocUTClass(0.2, 2);

}
#pragma endregion
#pragma endregion




// TODO: VECTOR UT

