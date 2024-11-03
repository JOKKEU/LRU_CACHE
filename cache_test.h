#include "cache.h"
#include <ctime>
#include <array>
#include <vector>
#include <chrono>
#include <thread>


#define EXIT_NULLPTR 2


struct test_struct__1
{
	static size_t temp;
	test_struct__1(size_t, size_t, size_t, size_t);
	size_t first_number;
	size_t second_number;
	size_t third_number;
	size_t fourth_number;
};


struct test_struct__2
{
	static size_t temp;
	test_struct__2();
	std::array<size_t, 5> arr;
};



void test_cache();