#pragma once
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <vector>
#include <thread>


#include "exc.h"
#include "wrapper.h"

#define _ERROR 3
#define _64BITE 64
#define EXIT_NULLPTR 2 

struct BUCKET
{
	size_t start_index;
	size_t current_index;
	size_t end_index;
	std::time_t old_used_data_time;
	std::time_t last_used_data_time;
	std::size_t size_bucket;
	Wrapper_for_cache* wrapper_old_used;
	size_t old_used_data_index;
};


struct Cache_LRU
{
	explicit Cache_LRU();
	~Cache_LRU();
	Cache_LRU(const Cache_LRU& other) = delete;
	Cache_LRU& operator=(const Cache_LRU&) = delete;
	void init_buckets_old_data_time();
	int replacement_unused_data(Wrapper_for_cache);
	int filling_cache(Wrapper_for_cache);
	void* init_data_for_cache(Wrapper_for_cache);
	void clear_cache();



private:
	const uint32_t _CACHE_SIZE = _64BITE * 4;
	std::time_t _old_time_used_in_cache;
	void** _all_memory;
	size_t _all_memory_size = _64BITE / 2;
	BUCKET _bucket1;
	BUCKET _bucket2;
	BUCKET _bucket3;
	BUCKET _bucket4;

};