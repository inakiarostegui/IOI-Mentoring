#include <stdio.h>
#include <iostream>
#include <utility>
#include <cstring>
#include <string>
#include <vector>

constexpr bool _debug = false;
const std::string PASS = "PASS";
const std::string FAIL = "FAIL";

void debug_print(const std::string& str)
{
    if (_debug)
        std::cout<<str<<std::endl;
}

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
class Vector/* : public BaseVector<T>*/
{
public:
    Vector() : m_size(0u), m_capacity(0u)
    {
        debug_print("DEFAULT CONSTRUCTED");
    }

    Vector(const unsigned initial_capacity) : m_size(0u)
    {
        debug_print("CONSTRUCTED ");

        reserve(initial_capacity);
    }

    ~Vector()
    {
        debug_print("DESTRUCTED ");

        delete [] m_container;
    }

    // Copy operator (DUMMY FUNCTION, to check unit tests)
    Vector(const Vector& other)
    {
        debug_print("COPY");
    }

    // Move operator
    Vector(Vector&& other) : m_container(other.m_container), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        debug_print("MOVE OPERATOR ");

        other.m_container = nullptr;
        other.m_capacity = 0u;
        other.m_size = 0u;
    }

    // Move assignment operator
    Vector& operator=(Vector&& other)
    {
        debug_print("MOVE ASSIGNMENT");
        
        if (this != &other)
        {
            delete [] m_container;

            m_container = other.m_container;
            m_capacity = other.m_capacity;
            m_size = other.m_size;

            other.m_container = nullptr;
            other.m_capacity = 0u;
            other.m_size = 0u;
        }
        return *this;
    }

    void push_back(const T& value)
    {
        if (m_size + 1u > m_capacity)
            grow(/*constant*/);

        m_container[m_size] = value;

        m_size++;
    }

    void pop_back()
    {
        // No need to modify memory, we can just decrease the size
        if (!empty())
            m_size--;
        else
            std::cout<<"WARNING [main.cpp, Vector, T pop_back()]: Vector was empty." << std::endl;
    }

    // Resizes and grows if required
    void resize(const unsigned new_size)
    {
        m_size = new_size;

        if (m_size > m_capacity)
            grow();
    }

    // Reserves memory by copying to a new vector of the required capacity
    void reserve(const unsigned new_capacity)
    {
        if (new_capacity > m_capacity)
        {
            T* temp_container = m_container;

            m_capacity = new_capacity;
            m_container = new T[new_capacity]();

            if (temp_container != nullptr)
            {
                std::memcpy(m_container, temp_container, sizeof(T) * m_size);    // returns desination
                delete [] temp_container;
            }
        }
        else
            std::cout<<"WARNING [main.cpp, Vector, void reserve()]: New capacity was not larger than previous capacity." << std::endl;

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

    void grow(/*const int new_size*/){};


private:
    T* m_container = nullptr;
    unsigned m_size = 0u;
    unsigned m_capacity = 0u;
};

bool unit_test_move_assignment()
{
    debug_print("-------------------------");

    Vector<int> vec_0{};
    Vector<int> vec_1{15};

    // Using move assignment operator
    vec_0 = std::move(vec_1);

    return vec_0.capacity() == 15;
}

bool unit_test_move_semantics_0()
{
    debug_print("-------------------------");

    Vector<int> vec_2{5};

    // Using move semantics
    Vector<int> vec_3(std::move(vec_2));

    return vec_3.capacity() == 5;
}

bool unit_test_move_semantics_1()
{
    debug_print("-------------------------");

    // Using move semantics
    BaseVector<int> bvec_0(10);

    return bvec_0.m_data.capacity() == 10;
}

class UnitTest
{
    public:
        UnitTest(const std::string& test_name, const bool test_result) : m_test_name(test_name), m_test_result(test_result)
        {    }
        
        UnitTest(const UnitTest& other) : m_test_name(other.m_test_name), m_test_result(other.m_test_result)
        {
            if (_debug)
                std::cout<<"UNIT TEST: COPY CONSTRUCTOR"<<std::endl;
        }

        UnitTest(UnitTest&& other) : m_test_name(other.m_test_name), m_test_result(other.m_test_result)
        {
            other.m_test_name = "";
            other.m_test_result = false;

            if (_debug)
                std::cout<<"UNIT TEST: MOVE CONSTRUCTOR"<<std::endl;
        }

        std::string m_test_name{};
        bool m_test_result = false;

};

void print_unit_test_results(std::vector<UnitTest>&& unit_test_results)
{
    debug_print("-------------------------");

    for (unsigned i = 0u; i < unit_test_results.size(); i++)
    {
        std::cout<< unit_test_results[i].m_test_name<<": ";
        std::cout<< (unit_test_results[i].m_test_result ? PASS : FAIL);
        std::cout<< std::endl;
    }
}

int main()
{
    // Option 1: Using copy constructors
    /*print_unit_test_results(    // Calling copy constructor
    {
        {"MOVE ASSIGNMENT", unit_test_move_assignment()},
        {"MOVE SEMANTICS 0", unit_test_move_semantics_0()},
        {"MOVE SEMANTICS 1", unit_test_move_semantics_1()}
    });*/

    // Option 2: Using move constructors
    std::vector<UnitTest> unit_test_results{};
    unit_test_results.reserve(3);

    unit_test_results.push_back(UnitTest("MOVE ASSIGNMENT", unit_test_move_assignment()));
    unit_test_results.push_back(UnitTest("MOVE SEMANTICS 0", unit_test_move_semantics_0()));
    unit_test_results.push_back(UnitTest("MOVE SEMANTICS 1", unit_test_move_semantics_1()));

    print_unit_test_results(std::move(unit_test_results));
    
    return 0;
}