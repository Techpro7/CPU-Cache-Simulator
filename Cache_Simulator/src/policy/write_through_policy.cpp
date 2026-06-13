#include"WritePolicy.h"
#include "memory.h"

class WriteThroughPolicy: public WritePolicy{
    public:
        WriteThroughPolicy(){}

        void LoadFromMemory(CacheLine& line,std::uint32_t address, Memory& memory){
            std::uint32_t block_start = address& ~(line.data.size()-1);
            for(auto i{0};i<line.data.size();++i)
            {
                line.data[i] = memory.read(block_start + i); 
            }

        }
        void write(CacheLine& line, std::uint32_t address, std::uint8_t value, AddressParts& parts, Memory& memory) {
            if(line.valid && line.tag == parts.tag)
            {
                line.data[parts.offset]=value;
            }
            else{
                line.valid = true;
                line.tag = parts.tag;
                LoadFromMemory(line,address,memory);
                line.data[parts.offset] = value;
            }
            memory.write(address,value); // write through to memory
        }
};