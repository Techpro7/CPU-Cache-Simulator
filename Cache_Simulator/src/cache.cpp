#include "cache.h"
#include<cstdint>

std::uint32_t Cache::blockAddress(std::uint32_t address)
{
    return address & ~(BlockSize-1);
}

std::uint8_t Cache::read(std::uint32_t address){
    AddressParts parts = decoder->DecodeAddress(address, BlockSize, IndexBits, OffsetBits);

    CacheLine& line = cache[parts.index];
    
    if(line.valid && line.tag == parts.tag){
        ++ReadHits; // cache hit
    }

    else{
        ++ReadMisses;
        line.valid = true;
        line.tag = parts.tag;
        loader->LoadFromMemory(address, line, BlockSize, OffsetBits, IndexBits); // load the block from memory to cache
        
    }
    return line.data[parts.offset];
}

void Cache::write(std::uint32_t address, std::uint8_t value)
{
    AddressParts parts = decoder->DecodeAddress(address, BlockSize, IndexBits, OffsetBits);

    CacheLine& line = cache [parts.index];
    
    if(line.valid && line.tag == parts.tag){
        ++WriteHits;
        line.data[parts.offset] =value;
    }

    else{
        ++WriteMisses;
        line.valid = true;
        line.tag = parts.tag;
        loader->LoadFromMemory(address, line, BlockSize, OffsetBits, IndexBits) ;
        line.data[parts.offset] = value;
    }
    // write-through policy implementation
    memory.write(address, value); 
}

void Cache::printStats() const{
    std::double_t Readhit_rate = (ReadHits+ ReadMisses)>0 ? ( (static_cast<double>(ReadHits)/(ReadHits + ReadMisses))*100) : 0 ;
    std::cout<< "ReadHits: "<< ReadHits<<"\n";
    std::cout<< "ReadMisses: "<< ReadMisses<<"\n";
    std::cout<< "ReadHit Rate: "<< Readhit_rate<<"%\n";
    std::cout<<"\n";
    std::double_t Writehit_rate = (WriteHits + WriteMisses > 0 ? (static_cast<double>(WriteHits)/(WriteHits + WriteMisses))*100 : 0);
    std::cout<< "WriteHits: "<< WriteHits<<"\n";
    std::cout<< "WriteMisses: "<< WriteMisses<<"\n";
    std::cout<< "WriteHit Rate: "<< Writehit_rate<<"%\n";
    std::cout<<"\n";
    std::cout<< "Total Accesses: "<< (ReadHits + ReadMisses + WriteHits + WriteMisses)<<"\n";
    std::double_t Overall_hit_rate = (ReadHits + WriteHits) / static_cast<double>(ReadHits + ReadMisses + WriteHits + WriteMisses) * 100;
    std::cout<< "Overall Hit Rate: "<< Overall_hit_rate<<"%\n";

    std::cout<<"\n";
}