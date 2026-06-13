#include<cstdint>
#include "memory.h"

std::uint8_t Memory::read(std::uint32_t address){
     if(address< memory.size()){
            return memory[address];
        }
        else{
            throw std::out_of_range("Address out of range");
        }
}

void Memory::write(std::uint32_t address, std::uint8_t value)
{
    memory[address]=value;
}