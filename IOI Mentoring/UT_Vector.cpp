/***************************************************************************//**
 * @filename UT_Vector.cpp
 * @brief	 Contains the vector unit test function implementations.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"
#include "Vector.h"
#include "VectorTestClass.h"

namespace UT
{
    namespace Vectors
    {
        bool reserve_0()
        {
            Vector<int> vec;
            vec.reserve(16);

            return vec.capacity() == 16 && vec.size() == 0;
        }

        bool reserve_1()
        {
            Vector<int> vec;
            vec.reserve(16);
            vec.reserve(10);

            return vec.capacity() == 16 && vec.size() == 0;
        }

        bool push_back_0()
        {
            Vector<int> vec;
            vec.push_back(0);

            return vec.capacity() == 1 && vec.size() == 1 && vec[0] == 0;
        }

        bool push_back_1()
        {
            Vector<int> vec;
            vec.reserve(16);

            vec.push_back(0);

            return vec.capacity() == 16 && vec.size() == 1 && vec[0] == 0;;
        }

        bool push_back_2()
        {
            Vector<int> vec;
            vec.reserve(4);

            for (unsigned i = 0u; i < 5; i++)
                vec.push_back(0);

            return vec.capacity() == 8 && vec.size() == 5 && vec[0] == 0;
        }

        bool emplace_back_0()
        {
            Vector<VectorTestClass> vec;
            vec.emplace_back(0, 0.0f);

            return vec.capacity() == 1 && vec.size() == 1 && vec[0] == VectorTestClass{0, 0.0f};
        }

        bool emplace_back_1()
        {
            Vector<VectorTestClass> vec;
            vec.reserve(16);

            vec.emplace_back(0, 0.0f);

            return vec.capacity() == 16 && vec.size() == 1 && vec[0] == VectorTestClass{ 0, 0.0f };
        }

        bool emplace_back_2()
        {
            Vector<VectorTestClass> vec;
            vec.reserve(4);

            for (unsigned i = 0u; i < 5; i++)
                vec.emplace_back(0, 0.0f);

            return vec.capacity() == 8 && vec.size() == 5 && vec[0] == VectorTestClass{ 0, 0.0f };
        }

        bool pop_back_0()
        {
            Vector<int> vec;
            vec.pop_back();

            return vec.capacity() == 0 && vec.size() == 0;
        }

        bool pop_back_1()
        {
            Vector<int> vec;
            vec.push_back(0);
            vec.pop_back();

            return vec.capacity() == 1 && vec.size() == 0;
        }

        bool clear()
        {
            Vector<int> vec;
            vec.push_back(0);
            vec.clear();

            return vec.capacity() == 1 && vec.size() == 0;
        }

        bool subscript_0()
        {
            Vector<int> vec;
            vec.push_back(10);

            return vec[0] == 10;
        }

        bool prod()
        {
            Vector<VectorTestClass> vec;
            vec.reserve(6);
            for (int i = 0; i < 13; i++)
                vec.push_back(VectorTestClass(i, 5.0f));
            for (int i = 0; i < 4; i++)
                vec.pop_back();

            return vec.size() == 9 && vec.capacity() == 24;
        }
    }
}