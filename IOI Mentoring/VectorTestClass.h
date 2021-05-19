/***************************************************************************//**
 * @filename VectorTestClass.h
 * @brief    Contains the VectorTestClass class definition that is used by
 *           the Vector class.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"

class VectorTestClass
{
public:

    VectorTestClass() = default;

    ~VectorTestClass()
    {
        debug_print("VectorTestClass: DESTRUCTED");
    }

    VectorTestClass(int&& other_x, float&& other_y) : x(other_x), y(other_y)
    {
        debug_print("VectorTestClass: CUSTOM CONSTRUCTOR");
    }

    VectorTestClass(const VectorTestClass& other) : x(other.x), y(other.y)
    {
        debug_print("VectorTestClass: COPY CONSTRUCTOR");
    }

    VectorTestClass(VectorTestClass&& other) noexcept : x(other.x), y(other.y)
    {
        debug_print("VectorTestClass: MOVE OPERATOR");

        other.x = 0;
        other.y = 0.0f;
    }

    VectorTestClass& operator=(const VectorTestClass& other)
    {
        debug_print("VectorTestClass: COPY ASSIGNMENT");

        x = other.x;
        y = other.y;

        return *this;
    }

    bool operator==(const VectorTestClass& other) const
    {
        return x == other.x && y == other.y;
    }

    VectorTestClass& operator=(VectorTestClass&& other) noexcept
    {
        debug_print("VectorTestClass: MOVE ASSIGNMENT");

        if (this != &other)
        {
            x = other.x;
            y = other.y;

            other.x = 0;
            other.y = 0.0f;
        }
        return *this;
    }

    // Insertion operator
    friend std::ostream& operator<<(std::ostream& os, const VectorTestClass& data)
    {
        os << data.x << ' ' << data.y;
        return os;
    }

private:
    int x = 1;
    float y = 2.0f;
};