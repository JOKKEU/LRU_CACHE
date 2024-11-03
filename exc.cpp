#include "exc.h"

exc::exception::exception(uint32_t error_code) : 
    _error_code(error_code)
{}
exc::exception::~exception() = default;
void exc::exception::what() const
{
    if (_error_code == 5) {std::cout << "Error code: " << _error_code << " - is error allocate\n";}
    else {std::cout << "Unknown error code\n";}           
}
