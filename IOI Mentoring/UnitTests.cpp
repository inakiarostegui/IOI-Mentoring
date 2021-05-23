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

const std::string PASS = "PASS";
const std::string FAIL = "FAIL";

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
            UnitTest{"LINEAR INIT",         &unit_test_alloc_linear_init},
            UnitTest{"LINEAR ALLOCATE 0",   &unit_test_alloc_linear_allocate_0},
            UnitTest{"LINEAR ALLOCATE 1",   &unit_test_alloc_linear_allocate_1},
            UnitTest{"LINEAR FREE 0",       &unit_test_alloc_linear_free_0},
            UnitTest{"LINEAR INIT CLEAR",   &unit_test_alloc_linear_init_clear},
            UnitTest{"STACK INIT 0",        &unit_test_alloc_stack_init},
            UnitTest{"STACK ALLOCATE 0",    &unit_test_alloc_stack_allocate_0},
            UnitTest{"STACK ALLOCATE 1",    &unit_test_alloc_stack_allocate_1},
            UnitTest{"STACK FREE 0",        &unit_test_alloc_stack_free_0},
            UnitTest{"STACK FREE 1",        &unit_test_alloc_stack_free_1},
            UnitTest{"STACK CLEAR",         &unit_test_alloc_stack_clear},
            UnitTest{"STACK INIT CLEAR",    &unit_test_alloc_stack_init_clear},
        }
    ),

};

void run_unit_tests(std::vector<e_UnitTestTypes>&& test_types_to_run)
{
    for (unsigned i = 0u; i < test_types_to_run.size(); i++)
    {
        debug_print("--------------" + UT_TITLES[static_cast<int>(test_types_to_run[i])] + "--------------");

        for (unsigned j = 0u; j < unit_tests[test_types_to_run[i]].size(); j++)
        {
            std::cout << unit_tests[test_types_to_run[i]][j].m_test_name << ": " << std::endl;
            std::cout << "RESULT: " << (unit_tests[test_types_to_run[i]][j].m_test_fn() ? PASS : FAIL);
            std::cout << std::endl;
            debug_print("-------------------------");
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
    std::byte* buffer;
    la.Init(buffer, 48);

    return la.GetBufferSize() == 48u;
}

bool unit_test_alloc_linear_allocate_0()
{
    LinearAllocator la;
    std::byte* buffer;
    la.Init(buffer, 48);
    la.Allocate(20);

    return la.GetOffset() == 20u;
}

bool unit_test_alloc_linear_allocate_1()
{
    LinearAllocator la;
    std::byte* buffer;
    la.Init(buffer, 48);
    la.Allocate(20);
    la.Allocate(30);

    return la.GetOffset() == 20u;
}

bool unit_test_alloc_linear_free_0()
{
    LinearAllocator la;
    std::byte* buffer;
    la.Init(buffer, 48);
    la.Allocate(20);
    la.Free();

    return buffer == nullptr;
}

bool unit_test_alloc_linear_init_clear()
{
    LinearAllocator la;
    std::byte* buffer;
    la.Init(buffer, 48);
    la.Allocate(20);
    la.Init(buffer, 36);

    return la.GetBufferSize() == 36u;
}
#pragma endregion

#pragma region STACK ALLOCATOR UT
bool unit_test_alloc_stack_init()
{
    StackAllocator sa;
    std::byte* buffer;
    sa.Init(buffer, 48);

    return sa.GetBufferSize() == 48u;
}

bool unit_test_alloc_stack_allocate_0()
{
    StackAllocator sa;
    std::byte* buffer;
    sa.Init(buffer, 48);
    sa.Allocate(20);

    return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
}

bool unit_test_alloc_stack_allocate_1()
{
    StackAllocator sa;
    std::byte* buffer;
    sa.Init(buffer, 48);
    sa.Allocate(20);
    sa.Allocate(30);

    return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
}

bool unit_test_alloc_stack_free_0()
{
    StackAllocator sa;
    std::byte* buffer;
    sa.Init(buffer, 48);
    sa.Allocate(20);
    sa.Allocate(20);

    return sa.GetOffset() == 20 && sa.GetAmountOfBlocks() == 1u;
}

bool unit_test_alloc_stack_free_1()
{
    StackAllocator sa;
    std::byte* buffer;
    sa.Init(buffer, 48);
    sa.Free();

    return sa.GetBufferSize() == 48u;
}

bool unit_test_alloc_stack_clear()
{
    StackAllocator sa;
    std::byte* buffer;
    sa.Init(buffer, 48);
    sa.Clear();

    return buffer == nullptr;
}

bool unit_test_alloc_stack_init_clear()
{
    StackAllocator sa;
    std::byte* buffer;
    sa.Init(buffer, 48);
    sa.Allocate(20);
    sa.Init(buffer, 36);

    return sa.GetBufferSize() == 36u && sa.GetOffset() == 0u && sa.GetAmountOfBlocks() == 0;
}
#pragma endregion
#pragma endregion




// TODO: VECTOR UT

