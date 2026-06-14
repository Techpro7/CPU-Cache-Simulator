#ifndef LOADER_HEADER
#define LOADER_HEADER

#include "memory.h"
#include "CacheLine.h"
#include<vector> 
#include<cstdint>

class Loader{
    public:
        Memory& ram_memory;
        Loader() = delete;

        Loader(Memory& mem): ram_memory{mem} {};
        void LoadFromMemory(std::uint32_t address, CacheLine& line, std::int32_t block_size);
};


#endif