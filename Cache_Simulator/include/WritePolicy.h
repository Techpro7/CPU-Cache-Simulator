#ifndef WRITE_POLICY_HEADER
#define WRITE_POLICY_HEADER 

#include <cstdint>
#include "CacheLine.h"
#include "AddressParts.h"
#include "memory.h"
class WritePolicy{
    public: 
        virtual void write(CacheLine& line, ::uint32_t address, std::uint8_t value, AddressParts& parts, Memory& memory) = 0;
};


#endif