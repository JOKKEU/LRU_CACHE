#pragma once
#include <cstdint>
#include <iostream>

namespace exc
{
    struct exception
    {
        explicit exception(uint32_t);
        ~exception();
        void what() const;
    private:
        uint32_t _error_code;
    };
};