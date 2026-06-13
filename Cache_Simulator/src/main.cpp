#include "cache.h"
#include "memory.h"
#include "Decoder.h"
#include "Loader.h"
#include <fstream>
#include <iostream>
#include <string>



int main()
{
    Memory memory{1024 * 1024 * 4};
    AddressDecoder decoder;
    Loader loader{memory};

    Cache cache(
        1024 * 1024,
        128,
        memory,
        &decoder, 
        &loader
    );

    std::ifstream file("traces/trace.txt");

    if (!file)
    {
        std::cerr << "Failed to open trace file\n";
        return 1;
    }

    char op;

    while (file >> op)
    {
        if (op == 'R')
        {
            std::uint32_t address;

            file >> std::hex >> address;

            std::uint8_t data =
                cache.read(address);

            std::cout
                << "READ  Address: 0x"
                << std::hex << address
                << " Data: "
                << std::dec
                << static_cast<std::uint32_t>(data)
                << '\n';
        }
        else if (op == 'W')
        {
            std::uint32_t address;
            std::uint32_t value;

            file >> std::hex >> address;
            file >> std::dec >> value;

            cache.write(
                address,
                static_cast<std::uint8_t>(value)
            );

            std::uint8_t data = memory.read(address);
            if(value != data)
            {
                std::cerr << "Error: Memory value does not match written value at address 0x" << std::hex << address << "\n";
            }
            std::cout
                << "WRITE Address: 0x"
                << std::hex << address
                << " Value: "
                << std::dec
                << value
                << '\n';
        }
        else
        {
            std::cerr
                << "Unknown operation: "
                << op
                << '\n';
        }
    }

    cache.printStats();

    return 0;
}