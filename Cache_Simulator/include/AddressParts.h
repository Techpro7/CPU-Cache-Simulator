#ifndef ADDRESS_PARTS_HEADER
#define ADDRESS_PARTS_HEADER

#include<cstdint>


struct AddressParts{

    std::uint32_t tag;
    std::uint32_t index;
    std::uint32_t offset;
};

#endif 