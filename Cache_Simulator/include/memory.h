#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#include<vector>
#include<cstdint>


class Memory {
    private:
        std::vector<std::uint8_t> memory;
    public: 
        Memory(size_t  size)
        : memory(size, 0)
        {
            for(auto i{0};i<size;++i)
            {
                memory[i] = static_cast<std::uint8_t>(i%256);
            }
        }

        std::uint8_t read(std::uint32_t address);
        void write(std::uint32_t address, std::uint8_t value);
};


#endif
