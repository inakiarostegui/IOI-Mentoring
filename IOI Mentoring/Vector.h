/***************************************************************************//**
 * @filename Vector.h
 * @brief	 Custom vector class definition and overloaded operators.
 * @author   Inaki Arostegui
 ******************************************************************************/

#pragma once
#include "pch.h"
#include "StackAllocator.h"

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
    Vector() : m_size(0u)
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

    Vector(const Vector& other) : m_container(other.m_container), m_size(other.m_size)
    {
        debug_print("Vector: COPY CONSTRUCTOR");
    }

    Vector(Vector&& other) noexcept : m_container(other.m_container), m_size(other.m_size), m_allocator(other.m_allocator)
    {
        debug_print("Vector: MOVE OPERATOR");

        other.m_allocator = StackAllocator();
        other.m_container = nullptr;
        other.m_size = 0u;
    }

    Vector& operator=(Vector&& other) noexcept
    {
        debug_print("Vector: MOVE ASSIGNMENT OPERATOR");

        if (this != &other)
        {
            delete[] m_container;

            m_container = other.m_container;
            m_allocator = other.m_allocator;
            m_size = other.m_size;

            other.m_allocator = StackAllocator();
            other.m_container = nullptr;
            other.m_size = 0u;
        }
        return *this;
    }

    // Pushback and emplace back both look the same... Could we maybe make a function that encapsulates them?
    T* push_back(T&& value)
    {
        // If were out of space then grow
        check_and_grow();

        // Add the value to the back of the container
        m_allocator.Allocate(sizeof(T));
        (*this)[++m_size - 1] = std::forward<T>(value);

        // This gives errors, still need to understand it
        //new (m_allocator.Allocate(sizeof(T))) std::forward<T>(value);
        
        return &(*this)[m_size - 1];
    }

    T* push_back(const T& value)
    {
        check_and_grow();

        m_allocator.Allocate(sizeof(T));
        (*this)[++m_size - 1] = value;

        return &(*this)[m_size - 1];
    }

    template < typename ...Args >
    T* emplace_back(Args&&... args)
    {
        check_and_grow();
        
        new (m_allocator.Allocate(sizeof(T))) T(std::forward<Args>(args)...);

        return &(*this)[++m_size - 1];
    }

    void check_and_grow()
    {
        if (m_size == capacity())
            grow();
    }

    void pop_back()
    {
        // No need to modify memory, we can just decrease the size
        if (!empty())
        {
            m_size--;
            m_allocator.Free();
        }
        else
            std::cout << "ERROR [Vector.h, Vector, T pop_back()]: Vector was empty." << std::endl;
    }

    void clear()
    {
        m_allocator.Clear();
        m_size = 0u;
    }

    // Resizes vector and grows if required
    void resize(const unsigned new_size)
    {
        if (m_size == new_size)
            return;

        if (new_size > capacity())
        {
            m_size = new_size;
            grow();
            return;
        }

        if (m_size < new_size)
        {
            for (unsigned i = 0u; i < std::abs(m_size - new_size); i++)
                m_allocator.Free();
        }
        else
        {
            for (unsigned i = 0u; i < std::abs(m_size - new_size); i++)
                m_allocator.Allocate(sizeof(T));
        }

        m_size = new_size;
    }

    // Reserves memory by copying to a new vector of the required capacity
    void reserve(unsigned&& new_capacity)
    {
        if (new_capacity > capacity())
        {
            std::byte* temp_container = m_container;

            // Create new container and allocator based on container
            m_container = new std::byte[new_capacity * (sizeof(T) + sizeof(StackAllocator::StackAllocationFooter))]();
            m_allocator.Init(std::span{ reinterpret_cast<std::byte*>(m_container), new_capacity * (sizeof(T) + sizeof(StackAllocator::StackAllocationFooter)) });

            // If there was a container previously then copy the data to the new container and delete the old one
            if (temp_container != nullptr)
            {
                for (unsigned i = 0u; i < m_size; i++)
                    std::memcpy(m_allocator.Allocate(sizeof(T)), temp_container + i * sizeof(T), sizeof(T));
                // This can also be done like this and we dont have to have it in the loop:
                //std::memcpy(m_container, temp_container, sizeof(T) * m_size);

                delete[] temp_container;
            }
        }
        else
            debug_print("ERROR [Vector.h, Vector, void reserve()]: New capacity was not larger than previous capacity.");

        /*If n is greater than the current vector capacity, the function causes the container to reallocate its storage increasing its capacity to n (or greater).
        In all other cases, the function call does not cause a reallocation and the vector capacity is not affected.
        This function has no effect on the vector size and cannot alter its elements.*/
        /*https://www.cplusplus.com/reference/vector/vector/reserve/*/
    }

    size_t size() const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_allocator.GetBufferSize() / (sizeof(T) + sizeof(StackAllocator::StackAllocationFooter));
    }

    StackAllocator* allocator()
    {
        return &m_allocator;
    }

    T* container() const
    {
        return reinterpret_cast<T*>(m_container);
    }

    bool empty() const
    {
        return m_size == 0;
    }

    // If empty, grow allocates space for 1, else we grow taking into account the multiplier
    void grow()
    {
        reserve(capacity() == 0 ? 1 : capacity() * GROWTH_MULTIPLIER);
    }

    // Subscript operator
    T& operator[](const int index)
    {
        if (m_container == nullptr)
        {
            debug_print("ERROR [main.cpp, Vector, T& operator[](const int)]: Container was empty.");
            assert(0);
        }

        if (static_cast<unsigned>(index) >= m_size || index < 0)
        {
            debug_print("ERROR [main.cpp, Vector, T& operator[](const int)]: Index not in container.");
            assert(0);
        }

        // Take the stack allocator footers into account when getting the data
        return *reinterpret_cast<T*>(m_container + index * (sizeof(T) + sizeof(StackAllocator::StackAllocationFooter)));
    }

    #if DEBUG
    void print_stats()
    {
        debug_print("Size: " + std::to_string(m_size) + "  |  Capacity: " + capacity());
    }

    void print_contents()
    {
        for (int i = 0; i < m_size; i++)
            std::cout << (*this)[i] << " | ";
        std::cout << std::endl;
    }
    #endif

private:
    std::byte* m_container = nullptr;
    StackAllocator m_allocator;

    size_t m_size = 0;
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
