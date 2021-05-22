/***************************************************************************//**
 * @filename main.cpp
 * @brief	 Main.
 * @author   Inaki Arostegui
 ******************************************************************************/

#include "pch.h"
#include "UnitTests.h"

//class X
//{
//public:
//	virtual void CanTrigger1() const = 0;
//
//	virtual void CannotTrigger1() = 0;
//};
//
//class B : public X
//{
//	public:
//		void CanTrigger1() const {};
//		void CannotTrigger1() {};
//};
//
//void Trigger1(const X& x)
//{
//	x.CanTrigger1();
//	//x.CannotTrigger1();
//
//	const B bee;
//	bee.CanTrigger1();
//}
//
/*class Goose
{
public:
	int a;
	double w;
	short j;
};

class BaseDak
{
public:
	int l;
	virtual void fn1() {};	
};

class Dak : public BaseDak
{
public:
	void fn() {};
	void fn1() {};
	double b;
};
*/

int main(void)
{
	/*Goose a;
	std::cout << sizeof(Goose) << std::endl;
	std::cout << int(&(a.j)) - int(&(a)) + sizeof(a.w) << std::endl;
	std::cout << (sizeof(Goose) == (int(&(a.j)) - int(&(a)) + sizeof(a.w)) ? "PASS" : "FAIL") << std::endl;

	Dak b;
	std::cout << sizeof(BaseDak) << std::endl;
	std::cout << &(b.b) << std::endl;
	std::cout << &(b.l) << std::endl;
	std::cout << sizeof(int&) << std::endl;
	std::cout << sizeof(Dak) << std::endl;*/

	run_unit_tests();		// Run test divided by enums? map(enum, fn*)
	return 0;
}