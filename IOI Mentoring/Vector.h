/***************************************************************************//**
 * @filename Vector.h
 * @brief	 Custom vector class definition and overloaded operators.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "PoolAllocator.h"

#define GROWTH_MULTIPLIER 2u

template < typename U >
class Vector;

template < typename T >
class BaseVector
{
public:
    // Using std::move because an rvalue reference is an lvalue, we force it to use the move operator instead
    // of the copy constructor
    BaseVector(Vector<T>&& data) : m_data(std::move(data))
    {   }

    Vector<T> m_data;
};

template < typename T >
class Vector
{
public:
    Vector() : m_size(0u), m_capacity(0u)
    {
        debug_print("Vector: DEFAULT CONSTRUCTED");
    }

    Vector(unsigned&& initial_capacity) : m_size(0u)
    {
        debug_print("Vector: CONSTRUCTED ");

        reserve(std::move(initial_capacity));
    }

    ~Vector()
    {
        debug_print("Vector: DESTRUCTED");

        delete[] m_container;
    }

    Vector(const Vector& other) : m_container(other.m_container), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        debug_print("Vector: COPY CONSTRUCTOR");
    }

    Vector(Vector&& other) noexcept : m_container(other.m_container), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        debug_print("Vector: MOVE OPERATOR");

        other.m_container = nullptr;
        other.m_capacity = 0u;
        other.m_size = 0u;
    }

    Vector& operator=(Vector&& other) noexcept
    {
        debug_print("Vector: MOVE ASSIGNMENT OPERATOR");

        if (this != &other)
        {
            delete[] m_container;

            m_container = other.m_container;
            m_capacity = other.m_capacity;
            m_size = other.m_size;

            other.m_container = nullptr;
            other.m_capacity = 0u;
            other.m_size = 0u;
        }
        return *this;
    }

    // Pushback and emplace back both look the same... Could we maybe make a function that encapsulates them?
    decltype(auto) push_back(T&& value)
    {
        // If were out of space then grow
        check_and_grow();

        // Add the value to the back of the container
        m_container[m_size] = std::forward<T>(value);

        m_size++;

        // Return a pointer to the element we added
        return m_container + m_size - 1;
    }

    decltype(auto) push_back(const T& value)
    {
        check_and_grow();

        m_container[m_size] = value;

        m_size++;

        return m_container + m_size - 1;
    }

    template < typename ...Args >
    decltype(auto) emplace_back(Args&&... args)
    {
        check_and_grow();

        m_container[m_size] = T(std::forward<Args>(args)...);
        
        m_size++;

        return m_container + m_size - 1;
    }

    void check_and_grow()
    {
        if (m_size == m_capacity)
            grow();
    }

    void pop_back()
    {
        // No need to modify memory, we can just decrease the size
        if (!empty())
            m_size--;
        else
            std::cout << "WARNING [Vector.h, Vector, T pop_back()]: Vector was empty." << std::endl;
    }

    void clear()
    {
        m_size = 0u;
    }

    // Resizes vector and grows if required
    void resize(const unsigned new_size)
    {
        m_size = new_size;

        if (m_size > m_capacity)
            grow();
    }

    // Reserves memory by copying to a new vector of the required capacity
    void reserve(unsigned&& new_capacity)
    {
        if (new_capacity > m_capacity)
        {
            T* temp_container = m_container;

            m_capacity = new_capacity;
            m_container = new T[new_capacity]();

            if (temp_container != nullptr)
            {
                std::memcpy(m_container, temp_container, sizeof(T) * m_size);
                delete[] temp_container;
            }
        }
        else
            debug_print("WARNING [Vector.h, Vector, void reserve()]: New capacity was not larger than previous capacity.");

        /*If n is greater than the current vector capacity, the function causes the container to reallocate its storage increasing its capacity to n (or greater).
        In all other cases, the function call does not cause a reallocation and the vector capacity is not affected.
        This function has no effect on the vector size and cannot alter its elements.*/
        /*https://www.cplusplus.com/reference/vector/vector/reserve/*/
    }

    unsigned size() const
    {
        return m_size;
    }

    unsigned capacity() const
    {
        return m_capacity;
    }

    bool empty() const
    {
        return m_size == 0u;
    }

    // If empty, grow allocates space for 1, else we grow taking into account the multiplier
    void grow()
    {
        reserve(m_capacity == 0 ? 1 : m_capacity * GROWTH_MULTIPLIER);
    }

    // Subscript operator
    T& operator[](const int index)
    {
        if (static_cast<unsigned>(index) > m_size || index < 0)
        {
            debug_print("ERROR [main.cpp, Vector, T& operator[](const int)]: Index not in container.");
            assert(0);  // Is this the way to do things? Need to check up on that...
        }

        return m_container[index];
    }

    #if DEBUG
    void print_stats()
    {
        debug_print("Size: " + std::to_string(m_size) + "  |  Capacity: " + std::to_string(m_capacity));
    }

    void print_contents()
    {
        for (int i = 0; i < m_size; i++)
            std::cout << m_container[i] << " | ";
        std::cout << std::endl;
    }
    #endif

private:
    T* m_container = nullptr;
    //PoolAllocator m_allocator;

    unsigned m_size = 0u;
    unsigned m_capacity = 0u;
};

template <typename T, typename U>
decltype(auto) push_back(T& vec, U&& data)
{
    return vec.push_back(std::forward<U>(data));
}

template <typename T, typename ...U>
decltype(auto) emplace_back(T& vec, U&&... data)
{
    return vec.emplace_back(std::forward<U>(data)...);
}

// Perfect Forwarding Example
/*class X
{
public:
    X(std::string other_x) : x(other_x) {}

    // Copy Constructor
    X(const X& other)
    {
        x = other.x;
        std::cout << "Copy Constructor" << x << std::endl;
    }
    // Move Constructor
    X(X&& other)
    {
        x = std::move(other.x);
        std::cout << "Move Constructor" << x << std::endl;
    }
private:
    std::string x;
};

template<typename T>
void Y(T&& param)
{
    X y(std::forward<T>(param));
}*/
