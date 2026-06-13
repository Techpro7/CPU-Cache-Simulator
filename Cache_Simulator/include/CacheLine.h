#ifndef CACHE_LINE_HEADER
#define CACHE_LINE_HEADER

#include<vector>
#include<cstdint>

struct CacheLine{
    bool valid{false}; // a valid bit to indicate if the cache line contains the valid data 
    std::uint32_t tag{0}; // a tag to identify the block of memory that is stored in the cache line 
    std::vector<std::uint8_t> data;
};

#endif 