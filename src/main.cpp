#include "assert/advanced_assert.h"

#include <iostream>

int main(int argc, char* argv[])
{
	// Testing that the library is in fact linked correctly by calling a function from it
	AdvancedAssert::setLoggingFunc([](const char* msg) {
		std::cout << msg << '\n';
	});

	std::cout << "Hello!" << '\n';

	return 0;
}