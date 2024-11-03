#include "wrapper.h"

Void_ptr_wrapper::Void_ptr_wrapper(void* obj, type_struct type) :
    _data_ptr(obj),
    _type(type),
    _size_struct_in_void_ptr(sizeof(type_struct) + sizeof(void*) + sizeof(size_t))
{}
type_struct Void_ptr_wrapper::get_type()
{
    return _type;
}

void* Void_ptr_wrapper::get_obj() 
{
    return _data_ptr;
}

size_t Void_ptr_wrapper::get_size_struct_in_void_ptr() const
{
    return _size_struct_in_void_ptr;
}


Wrapper_for_cache::Wrapper_for_cache(Void_ptr_wrapper& vpw, std::time_t time_last_use = 0) :
    _vpw(vpw),
    _time_last_use(time_last_use),
    _size_wrapper(_vpw.get_size_struct_in_void_ptr() + sizeof(std::time_t) + sizeof(size_t))
{}

std::time_t Wrapper_for_cache::get_time_last_use() const
{
    return _time_last_use;
}

void Wrapper_for_cache::set_time_last_use(std::time_t last)
{
    _time_last_use = last;
}

Void_ptr_wrapper* Wrapper_for_cache::get_vpw()
{
    return &_vpw;
}

size_t Wrapper_for_cache::get_size_wrapper() const
{
    return _size_wrapper;
}

