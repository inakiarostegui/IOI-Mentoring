/***************************************************************************//**
 * @filename UnitTest.h
 * @brief    Contains the Unit Test class definition.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"

class UnitTest
{
public:
    UnitTest(const std::string& test_name, bool (*test_fn)()) : m_test_name(test_name), m_test_fn(test_fn)
    {    }

    UnitTest(const UnitTest& other) : m_test_name(other.m_test_name), m_test_fn(other.m_test_fn)
    {    }

    UnitTest(UnitTest&& other) noexcept : m_test_name(other.m_test_name), m_test_fn(other.m_test_fn)
    {
        other.m_test_name = "";
        other.m_test_fn = nullptr;
    }

    std::string m_test_name{};
    bool (*m_test_fn)();
};

