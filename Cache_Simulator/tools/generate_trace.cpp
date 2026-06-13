#include <fstream>
#include <random>
#include <cstdint>
#include <iomanip>

int main()
{
    std::ofstream out("/traces/trace.txt");

    if (!out)
    {
        return 1;
    }

    std::mt19937 rng(42);

    // 80% accesses come from a small hot region
    std::uniform_int_distribution<uint32_t> hotAddr(
        0x1000, 0x10FF);

    // 20% accesses come from a larger region
    std::uniform_int_distribution<uint32_t> coldAddr(
        0x0000, 0xFFFF);

    // Read / Write selection
    std::uniform_int_distribution<int> opDist(0, 99);

    // Values for writes
    std::uniform_int_distribution<uint32_t> valueDist(
        0, 255);

    constexpr int NUM_OPERATIONS = 10000;

    for (int i = 0; i < NUM_OPERATIONS; ++i)
    {
        uint32_t address;

        if ((rng() % 100) < 80)
        {
            address = hotAddr(rng);
        }
        else
        {
            address = coldAddr(rng);
        }

        bool isWrite = (opDist(rng) < 30); // 30% writes

        if (isWrite)
        {
            uint32_t value = valueDist(rng);

            out << "W "
                << "0x"
                << std::hex << address
                << " "
                << std::dec << value
                << '\n';
        }
        else
        {
            out << "R "
                << "0x"
                << std::hex << address
                << '\n';
        }
    }

    return 0;
}