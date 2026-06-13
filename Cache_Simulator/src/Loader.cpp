#include "Loader.h"

void Loader::LoadFromMemory(std::uint32_t address, CacheLine& line, std::int32_t block_size, std::int32_t offset_bits, std::int32_t index_bits)
{
    std::uint32_t block_start = address & ~(block_size-1);
    for(auto i{0};i<block_size;++i)
    {
        line.data[i] = ram_memory.read(block_start + i);
        line.valid = true;
    }
}
      

