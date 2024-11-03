#include "cache_test.h"


size_t test_struct__1::temp = 1;
size_t test_struct__2::temp = 1;

test_struct__1::test_struct__1(size_t a = 1, size_t b = 1, size_t c = 1, size_t d = 1) :
	first_number(a + test_struct__1::temp),
	second_number(b + test_struct__1::temp),
	third_number(c + test_struct__1::temp),
	fourth_number(d + test_struct__1::temp)
{
	++test_struct__1::temp;
}


test_struct__2::test_struct__2()
{
	arr = {temp, temp, temp, temp, temp};
	++test_struct__2::temp;
}



static Wrapper_for_cache generate_test_struct__1_for_cache()
{
	test_struct__1 test(0,0,0,0);
	void* ptr_to_test = reinterpret_cast<void*>(&test);
	Void_ptr_wrapper vpw(reinterpret_cast<void*>(ptr_to_test), 1);
	Wrapper_for_cache wrapper_unit(vpw, 0);
	return wrapper_unit;
}

static Wrapper_for_cache generate_test_struct__2_for_cache()
{
	test_struct__2 test;
	void* ptr_to_test = reinterpret_cast<void*>(&test);
	Void_ptr_wrapper vpw(reinterpret_cast<void*>(ptr_to_test), 2);
	Wrapper_for_cache wrapper_unit(vpw, 0);
	return wrapper_unit;

}

static Void_ptr_wrapper* unwrapper(void* ptr_wrapper_unit)
{
	return reinterpret_cast<Void_ptr_wrapper*>(reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_vpw());
}

static test_struct__1* deploy_test1(Void_ptr_wrapper* vpw)
{
	return reinterpret_cast<test_struct__1*>(vpw->get_obj());
}

static test_struct__2* deploy_test2(Void_ptr_wrapper* vpw)
{
	return reinterpret_cast<test_struct__2*>(vpw->get_obj());
}


 
void test_cache()
{
	/*

	void* ptr = generate_test_struct__2_for_cache();
	
	Void_ptr_wrapper* vpw = unwrapper(ptr);
	if (vpw->get_type() == 1)
	{
		test_struct__1* result = deploy_test1(vpw);
		std::cout << "test_struct__1\n";
		std::cout << result->first_number << std::endl;
		std::cout << result->third_number << std::endl;
	}
	else if (vpw->get_type() == 2)
	{
		test_struct__2* result = deploy_test2(vpw);
		std::cout << "test_struct__2\n";
		for (size_t i = 0; i < 5; i++)
		{
			std::cout << result->arr[i] << " ";
		}
	}
	
	*/


	std::srand(std::time(nullptr));
	Cache_LRU cache;
	std::vector<Wrapper_for_cache> wrapper_for_test;

	for (size_t i = 0 ; i < 50; ++i)
	{
		int random_value = std::rand() % 2;
		//if (random_value == 1)
		Wrapper_for_cache wrapper_unit1 = generate_test_struct__1_for_cache();
		wrapper_for_test.push_back(wrapper_unit1);
	} 


	while(true)
	{
		int random_value = std::rand() % 50;
		if (cache.filling_cache(wrapper_for_test[random_value]) == EXIT_FAILURE) 
		{
			cache.replacement_unused_data(wrapper_for_test[random_value]);
		}
		else if (cache.filling_cache(wrapper_for_test[random_value]) == EXIT_NULLPTR)
		{
			continue;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	

	return;
}