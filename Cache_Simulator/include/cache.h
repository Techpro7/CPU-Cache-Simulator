#ifndef CACHE_HEADER
#define CACHE_HEADER

#include "CacheLine.h"
#include "memory.h"
#include "AddressParts.h"
#include "Decoder.h"
#include "Loader.h"
#include<iostream>
#include<vector>
#include<cmath>
#include<cstdint>

class Cache{
    private: 
        std::vector<CacheLine> cache;
        Memory& memory;
        // WritePolicy& write_through_policy;

        std::int32_t ReadHits{0};
        std::int32_t WriteHits{0};
        std::int32_t ReadMisses{0};
        std::int32_t WriteMisses{0};

        std::int32_t BlockSize{0};
        std::int32_t OffsetBits{0};
        std::int32_t NumCacheLines{0};
        std::int32_t IndexBits{0};
        std::int32_t TotalTagBits{0};

        AddressDecoder* decoder;
        Loader* loader;
    
        // AddressParts DecodeAddress(std::uint32_t address);
        std::uint32_t blockAddress(std::uint32_t address);
        // void LoadFromMemory(std::uint32_t address);

    public:

        Cache(std::int32_t cache_size, std::int32_t block_size , Memory& mem, AddressDecoder* add_decoder, Loader* data_loader)
            :   BlockSize{block_size},
                OffsetBits{static_cast<std::int32_t>(std::log2(block_size))}, 
                NumCacheLines{cache_size/block_size},
                IndexBits{static_cast<std::int32_t>(std::log2(NumCacheLines))},
                TotalTagBits{32 - OffsetBits - IndexBits},
                memory{mem},
                decoder(add_decoder),
                loader(data_loader)
        {  
            cache.resize(NumCacheLines); // initialize the cache with the number of cache lines
            for(auto i{0};i<NumCacheLines;++i)
            {
                cache[i].data.resize(block_size);
            }
        }


        std::uint8_t read(std::uint32_t address);

        void write(std::uint32_t address, std::uint8_t value);

        void printStats() const;
        
};


#endif 
