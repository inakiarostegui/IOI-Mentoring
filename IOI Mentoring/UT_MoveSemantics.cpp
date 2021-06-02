/***************************************************************************//**
 * @filename UT_MoveSemantics.cpp
 * @brief	 Contains the move semantics unit test function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "Vector.h"
#include "VectorTestClass.h"

namespace UT
{
    namespace MoveSemantics
    {
        #pragma region VECTOR MOVE SEMANTICS UT
        bool move_assignment()
        {
            Vector<int> vec_0{};
            Vector<int> vec_1{ 15 };

            // Using move assignment operator
            vec_0 = std::move(vec_1);

            return vec_0.capacity() == 15;
        }

        bool move_semantics_0()
        {
            Vector<int> vec_2{ 5 };

            // Using move semantics
            Vector<int> vec_3(std::move(vec_2));

            return vec_3.capacity() == 5;
        }

        bool move_semantics_1()
        {
            // Using move semantics
            BaseVector<int> bvec_0(10);

            return bvec_0.m_data.capacity() == 10;
        }
        #pragma endregion

        #pragma region PERFECT FORWARDING UT
        #pragma region PUSH UT
        bool push_rvalue_0()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;

            vec_last = vec.push_back(VectorTestClass(3, 4.0f));

            return *vec_last == VectorTestClass(3, 4.0f) && vec_last == &(vec[vec.size() - 1]);
        }

        bool push_rvalue_1()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(11, 22.0f);

            vec_last = vec.push_back(std::move(vec_test));

            return *vec_last == VectorTestClass(11, 22.0f) && VectorTestClass(0, 0.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
        }

        bool push_rvalue_2()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(3, -1.0f);

            vec_last = push_back(vec, std::move(vec_test));

            return *vec_last == VectorTestClass(3, -1.0f) && VectorTestClass(0, 0.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
        }

        bool push_lvalue_0()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(9, 2.0f);

            vec_last = vec.push_back(vec_test);

            return *vec_last == VectorTestClass(9, 2.0f) && VectorTestClass(9, 2.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
        }

        bool push_lvalue_1()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(3, -1.0f);

            vec_last = push_back(vec, vec_test);

            return *vec_last == VectorTestClass(3, -1.0f) && VectorTestClass(3, -1.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
        }
        #pragma endregion

        #pragma region EMPLACE UT
        bool emplace_lvalue_0()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(9, -3.0f);

            vec_last = vec.emplace_back(vec_test);

            return *vec_last == VectorTestClass(9, -3.0f) && VectorTestClass(9, -3.0f) == vec_test && vec_last == &(vec[vec.size() - 1]);
        }

        bool emplace_lvalue_1()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(13, 6.0f);

            vec_last = emplace_back(vec, vec_test);

            return *vec_last == VectorTestClass(13, 6.0f) && vec_test == VectorTestClass(13, 6.0f) && vec_last == &(vec[vec.size() - 1]);
        }

        bool emplace_rvalue_0()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;

            vec_last = vec.emplace_back(7, 1.0f);

            return *vec_last == VectorTestClass(7, 1.0f) && vec_last == &(vec[vec.size() - 1]);
        }

        bool emplace_rvalue_1()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(6, 12.0f);

            vec_last = vec.emplace_back(std::move(vec_test));

            return *vec_last == VectorTestClass(6, 12.0f) && vec_last == &(vec[vec.size() - 1]) && vec_test == VectorTestClass(0, 0.0f);
        }

        bool emplace_rvalue_2()
        {
            Vector<VectorTestClass> vec;
            VectorTestClass* vec_last = nullptr;
            VectorTestClass vec_test(6, 12.0f);

            vec_last = emplace_back(vec, std::move(vec_test));

            return *vec_last == VectorTestClass(6, 12.0f) && vec_last == &(vec[vec.size() - 1]) && vec_test == VectorTestClass(0, 0.0f);
        }
        #pragma endregion
        #pragma endregion
    }
}