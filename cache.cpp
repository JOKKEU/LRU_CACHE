#include "cache.h"


static
std::time_t min_time(std::time_t first, std::time_t second, std::time_t third, std::time_t fourth)
{
	std::time_t min_value = first;
	min_value = std::min(min_value, second);
	min_value = std::min(min_value, third);
	min_value = std::min(min_value, fourth);

	return min_value;
}


Cache_LRU::Cache_LRU()
{
	_all_memory = reinterpret_cast<void**>(malloc(_CACHE_SIZE));
	if(_all_memory == nullptr) {exc::exception(5);}


	_bucket1.size_bucket = _64BITE / 8;
	_bucket1.start_index = 0;
	_bucket1.current_index = _bucket1.start_index;
	_bucket1.end_index = _bucket1.size_bucket;
	_bucket1.old_used_data_time = 0;
	_bucket1.last_used_data_time = 0;
	_bucket1.wrapper_old_used = nullptr;
	_bucket1.old_used_data_index = 0;

	_bucket2.size_bucket = _64BITE / 8;
	_bucket2.start_index = _bucket2.size_bucket;
	_bucket2.current_index = _bucket2.start_index;
	_bucket2.end_index = _bucket1.size_bucket + _bucket2.size_bucket;
	_bucket2.old_used_data_time = 0;
	_bucket2.last_used_data_time = 0;
	_bucket2.wrapper_old_used = nullptr;
	_bucket2.old_used_data_index = 0;


	_bucket3.size_bucket = _64BITE / 8;
	_bucket3.start_index = _bucket3.size_bucket + _bucket3.size_bucket;
	_bucket3.current_index = _bucket3.start_index;
	_bucket3.end_index = _bucket1.size_bucket + _bucket2.size_bucket + _bucket3.size_bucket;
	_bucket3.old_used_data_time = 0;
	_bucket3.last_used_data_time = 0;
	_bucket3.wrapper_old_used = nullptr;
	_bucket3.old_used_data_index = 0;


	_bucket4.size_bucket = _64BITE / 8;
	_bucket4.start_index = _bucket4.size_bucket + _bucket4.size_bucket + _bucket3.size_bucket;
	_bucket4.current_index = _bucket4.start_index;
	_bucket4.end_index = _all_memory_size;
	_bucket4.old_used_data_time = 0;
	_bucket4.last_used_data_time = 0;
	_bucket4.wrapper_old_used = nullptr;
	_bucket4.old_used_data_index = 0;

	std::cout << "cache created and initialized\n";
	std::cout <<  "bucket1(" << _bucket1.size_bucket << ") :" << _bucket1.start_index << " - " << _bucket1.end_index << std::endl;
	std::cout <<  "bucket2(" << _bucket2.size_bucket << ") :" << _bucket2.start_index << " - " << _bucket2.end_index << std::endl;
	std::cout <<  "bucket3(" << _bucket3.size_bucket << ") :" << _bucket3.start_index << " - " << _bucket3.end_index << std::endl;
	std::cout <<  "bucket4(" << _bucket4.size_bucket << ") :" << _bucket4.start_index << " - " << _bucket4.end_index << std::endl;
}



void Cache_LRU::clear_cache()
{
	for (size_t index = _bucket1.start_index; index < _bucket4.end_index; ++index)
	{
		free(_all_memory[index]);
	}
}



Cache_LRU::~Cache_LRU()
{
	clear_cache();
	free(_all_memory);

	std::cout << "cache cleared\n";
}


void* Cache_LRU::init_data_for_cache(Wrapper_for_cache wrapper_unit)
{
	wrapper_unit.set_time_last_use(std::time(nullptr));
	for (size_t index_wrapper = _bucket1.start_index; index_wrapper < _bucket4.end_index; ++index_wrapper)
	{
		if (memcmp(_all_memory[index_wrapper], &wrapper_unit, sizeof(&wrapper_unit) == 1)) {return nullptr;} 
	}
	void* ptr_wrapper_unit = reinterpret_cast<void*>(malloc(wrapper_unit.get_size_wrapper()));
	if (ptr_wrapper_unit == nullptr) {exc::exception(5);}
	memcpy(ptr_wrapper_unit, &wrapper_unit, wrapper_unit.get_size_wrapper()); 
	return ptr_wrapper_unit;
}


int Cache_LRU::filling_cache(Wrapper_for_cache wrapper_unit)
{
	try
	{
		void* ptr_wrapper_unit = init_data_for_cache(wrapper_unit);
		if (ptr_wrapper_unit == nullptr) {return EXIT_NULLPTR;}
		if (_bucket1.current_index < _bucket1.end_index)
		{
			std::cout << "bucket1 current_index: " << _bucket1.current_index << std::endl;
			_all_memory[_bucket1.current_index] = ptr_wrapper_unit;
			_bucket1.current_index++;
			_bucket1.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			std::cout << "wrapper appended\n";
			return EXIT_SUCCESS;
		}

		else if (_bucket2.current_index < _bucket2.end_index)
		{
			std::cout << "bucket2 current_index: " << _bucket2.current_index << std::endl;
			_all_memory[_bucket2.current_index] = ptr_wrapper_unit;
			_bucket2.current_index++;
			_bucket2.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			std::cout << "wrapper appended\n";
			return EXIT_SUCCESS;
		}
		else if (_bucket3.current_index < _bucket3.end_index)
		{
			std::cout << "bucket3 current_index: " << _bucket3.current_index << std::endl;
			_all_memory[_bucket3.current_index] = ptr_wrapper_unit;
			_bucket3.current_index++;
			_bucket3.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			std::cout << "wrapper appended\n";
			return EXIT_SUCCESS;
		}
		else if (_bucket4.current_index < _bucket4.end_index)
		{
			std::cout << "bucket4 current_index: " << _bucket4.current_index << std::endl;
			_all_memory[_bucket4.current_index] = ptr_wrapper_unit;
			_bucket4.current_index++;
			_bucket4.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			std::cout << "wrapper appended\n";
			return EXIT_SUCCESS;
		}
		else
		{
			std::cout << "cache is filled\n";
			return EXIT_FAILURE;
		}
	}
	catch (const exc::exception& ex)
	{
		ex.what();
		exit(EXIT_FAILURE);
	}
}


void Cache_LRU::init_buckets_old_data_time()
{
	{
		std::cout << "\ninitializing old items in buckets\n";
		std::time_t bucket1_temp_min_time = _bucket1.last_used_data_time; 
		Wrapper_for_cache* bucket1_struct_old_time = nullptr;
		size_t bucket1_temp_old_index = 0;
		for (size_t index_wrapper = _bucket1.start_index; index_wrapper < _bucket1.end_index; ++index_wrapper)
		{
		 	if (reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use() < bucket1_temp_min_time)
		 	{
		 		bucket1_temp_min_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use();
		 		bucket1_struct_old_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper]);
		 		bucket1_temp_old_index = index_wrapper;
		 	}
		}

		_bucket1.wrapper_old_used = bucket1_struct_old_time;
		_bucket1.old_used_data_time = bucket1_temp_min_time;
		_bucket1.old_used_data_index = bucket1_temp_old_index;

		std::cout << "bucket1 old_used_data_time -> " << _bucket1.old_used_data_time << std::endl;
		std::cout << "bucket1 old_used_data_index -> " << _bucket1.old_used_data_index << std::endl;
		std::cout << "bucket1 ok\n";

	}

	{
		std::time_t bucket2_temp_min_time = _bucket2.last_used_data_time; 
		Wrapper_for_cache* bucket2_struct_old_time = nullptr;
		size_t bucket2_temp_old_index = 0;
		for (size_t index_wrapper = _bucket2.start_index; index_wrapper < _bucket2.end_index; ++index_wrapper)
		{
		 	if (reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use() < bucket2_temp_min_time)
		 	{
		 		bucket2_temp_min_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use();
		 		bucket2_struct_old_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper]);
		 		bucket2_temp_old_index = index_wrapper;
		 	}
		}
		_bucket2.wrapper_old_used = bucket2_struct_old_time;
		_bucket2.old_used_data_time = bucket2_temp_min_time;
		_bucket2.old_used_data_index = bucket2_temp_old_index;

		std::cout << "bucket2 old_used_data_time -> " << _bucket2.old_used_data_time << std::endl;
		std::cout << "bucket2 old_used_data_index -> " << _bucket2.old_used_data_index << std::endl;
		std::cout << "bucket2 ok\n";
	}

	{
		std::time_t bucket3_temp_min_time = _bucket3.last_used_data_time; 
		Wrapper_for_cache* bucket3_struct_old_time = nullptr;
		size_t bucket3_temp_old_index = 0;
		for (size_t index_wrapper = _bucket3.start_index; index_wrapper < _bucket3.end_index; ++index_wrapper)
		{
		 	if (reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use() < bucket3_temp_min_time)
		 	{
		 		bucket3_temp_min_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use();
		 		bucket3_struct_old_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper]);
		 		bucket3_temp_old_index = index_wrapper;
		 	}
		}
		_bucket3.wrapper_old_used = bucket3_struct_old_time;
		_bucket3.old_used_data_time = bucket3_temp_min_time;
		_bucket3.old_used_data_index = bucket3_temp_old_index;

		std::cout << "bucket3 old_used_data_time -> " << _bucket3.old_used_data_time << std::endl;
		std::cout << "bucket3 old_used_data_index -> " << _bucket3.old_used_data_index << std::endl;
		std::cout << "bucket3 ok\n";
	}

	{
		std::time_t bucket4_temp_min_time = _bucket4.last_used_data_time; 
		Wrapper_for_cache* bucket4_struct_old_time = nullptr;
		size_t bucket4_temp_old_index = 0;
		for (size_t index_wrapper = _bucket4.start_index; index_wrapper < _bucket4.end_index; ++index_wrapper)
		{
		 	if (reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use() < bucket4_temp_min_time) // index wrapper = 24 / segfault
		 	{
		 		bucket4_temp_min_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper])->get_time_last_use();
		 		bucket4_struct_old_time = reinterpret_cast<Wrapper_for_cache*>(_all_memory[index_wrapper]);
		 		bucket4_temp_old_index = index_wrapper;
		 	}
		}
		_bucket4.wrapper_old_used = bucket4_struct_old_time;
		_bucket4.old_used_data_time = bucket4_temp_min_time;
		_bucket4.old_used_data_index = bucket4_temp_old_index;
		std::cout << "bucket4 old_used_data_time -> " << _bucket4.old_used_data_time << std::endl;
		std::cout << "bucket4 old_used_data_index -> " << _bucket4.old_used_data_index << std::endl;
		std::cout << "bucket4 ok\n";
	}

	std::cout << "old data in bucket1 -> " << _bucket1.old_used_data_time  << "(time_t)\n";
	std::cout << "old data in bucket2 -> " << _bucket2.old_used_data_time  << "(time_t)\n";
	std::cout << "old data in bucket3 -> " << _bucket3.old_used_data_time  << "(time_t)\n";
	std::cout << "old data in bucket4 -> " << _bucket4.old_used_data_time  << "(time_t)\n";

	_old_time_used_in_cache = min_time(_bucket1.old_used_data_time, _bucket2.old_used_data_time, _bucket3.old_used_data_time, _bucket4.old_used_data_time);

	std::cout << "old data in all_cache -> " << _old_time_used_in_cache << "(time_t)\n";
}


int Cache_LRU::replacement_unused_data(Wrapper_for_cache wrapper_unit)
{
	try
	{
		std::cout << "\nreplacement old data\n";
		void* ptr_wrapper_unit = init_data_for_cache(wrapper_unit);
		init_buckets_old_data_time();

		if (_bucket1.old_used_data_time == _old_time_used_in_cache)
		{
			std::cout << "bucket1 remove old data -> " << _bucket1.wrapper_old_used->get_time_last_use() << "(time_t)\n";
			free(_bucket1.wrapper_old_used);
			_all_memory[_bucket1.old_used_data_index] = ptr_wrapper_unit;
			_bucket1.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			_bucket1.wrapper_old_used = nullptr;
			return EXIT_SUCCESS;

		}

		else if (_bucket2.old_used_data_time == _old_time_used_in_cache)
		{
			std::cout << "bucket2 remove old data -> " << _bucket2.wrapper_old_used->get_time_last_use() << "(time_t)\n";
			free(_bucket2.wrapper_old_used);
			_all_memory[_bucket2.old_used_data_index] = ptr_wrapper_unit;
			_bucket2.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			_bucket2.wrapper_old_used = nullptr;

			return EXIT_SUCCESS;

		}
		else if (_bucket3.old_used_data_time == _old_time_used_in_cache)
		{
			std::cout << "bucket3 remove old data -> " << _bucket3.wrapper_old_used->get_time_last_use() << "(time_t)\n";
			free(_bucket3.wrapper_old_used);
			_all_memory[_bucket1.old_used_data_index] = ptr_wrapper_unit;
			_bucket3.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			_bucket3.wrapper_old_used = nullptr;

			return EXIT_SUCCESS;

		}
		else if (_bucket4.old_used_data_time == _old_time_used_in_cache)
		{
			std::cout << "bucket4 remove old data -> " << _bucket4.wrapper_old_used->get_time_last_use() << "(time_t)\n";
			free(_bucket4.wrapper_old_used);
			_all_memory[_bucket1.old_used_data_index] = ptr_wrapper_unit;
			_bucket4.last_used_data_time = reinterpret_cast<Wrapper_for_cache*>(ptr_wrapper_unit)->get_time_last_use();
			_bucket4.wrapper_old_used = nullptr;

			return EXIT_SUCCESS;
		}

		else
		{
			return EXIT_FAILURE;
		}

	}
	catch (const exc::exception& ex)
	{
		ex.what();
		exit(EXIT_FAILURE);
	}
}


