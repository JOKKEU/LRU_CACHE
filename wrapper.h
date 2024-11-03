#pragma once
#include <cstdint>
#include <ctime>
#include <iostream>

#include "exc.h"

typedef uint32_t type_struct;

#pragma pack(push, 1)
struct Void_ptr_wrapper
{
    explicit Void_ptr_wrapper(void*, type_struct);
    ~Void_ptr_wrapper() = default;
    type_struct get_type();
    void* get_obj();
    size_t get_size_struct_in_void_ptr() const;


private:
    void* _data_ptr;
    type_struct _type;
    size_t _size_struct_in_void_ptr;
};


struct Wrapper_for_cache
{
    explicit Wrapper_for_cache(Void_ptr_wrapper&, std::time_t);
    ~Wrapper_for_cache() = default;
    std::time_t get_time_last_use() const;
    void set_time_last_use(std::time_t);
    Void_ptr_wrapper* get_vpw();
    size_t get_size_wrapper() const;

private:
    Void_ptr_wrapper _vpw;
    std::time_t _time_last_use;
    size_t _size_wrapper;
};

#pragma pack(pop)