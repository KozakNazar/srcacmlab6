#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/**********************************************
* N.Kozak // Lviv'2018 // ACM RxCpp example   *
*    file: acmlab6.cpp                        *
***********************************************/
//#pragma once
//#include "stdafx.h"

#define WORK_SPACE int a = 0, b = 0
#define EXPRESSION ( C2*a + C1*b + C0 )
#define RUN_FOR_UPDATE EXPRESSION
#define C2 4
#define C1 4
#define C0 4

#include "rxcpp/rx.hpp"
namespace rx = rxcpp;
namespace rxsub = rxcpp::subjects;
namespace rxu = rxcpp::util;
#include <cctype>
#include <clocale>

WORK_SPACE; // r = C2*a + C1*b + C0;

void runForUpdateA(int value){
	a = value;
}
void runForUpdateB(int value){
	b = value;
}
void runForUpdateR(){
	std::cout << "(C2=" << C2 << " C1=" << C1 << " C0=" << C0 << " a=" << a << " b=" << b << ")" << std::endl;
	std::cout << "r = C2*a + C1*b + C0 = " << RUN_FOR_UPDATE << std::endl;
}
int main()
{
	std::cout << "(use key a, b, r; use <Ctrl+C> for exit)" << std::endl << "Run('r'<= merge('a', 'b')):" << std::endl;
	auto keys = rx::observable<>::create<int>(
		[](rx::subscriber<int> dest){
		for (;;) {
			int key = std::cin.get(), value;
			if (std::tolower(key) == 'a') {
				std::cout << "=";
				std::cin >> value;
				runForUpdateA(value);
			}
			else if (std::tolower(key) == 'b') {
				std::cout << "=";
				std::cin >> value;
				runForUpdateB(value);
			}
			dest.on_next(key);
		}
	}).
		publish();

	auto a = keys.
		filter([](int key){return std::tolower(key) == 'a'; });

	auto b = keys.
		filter([](int key){return std::tolower(key) == 'b'; });

	auto r = keys.
		filter([](int key){return std::tolower(key) == 'r'; });

	r.merge(a, b).
		subscribe([](int key){
		if (std::tolower(key) == 'r'){
			runForUpdateR();
		}
		else{
			std::cout << "value updated" << std::endl;
		}
	});

	keys.connect(); // run the loop in create

	return 0;
}