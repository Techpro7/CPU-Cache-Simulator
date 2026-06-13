#ifndef DECODER_HEADER
#define DECODER_HEADER

#include "AddressParts.h"


class AddressDecoder{
    public:
        AddressParts parts;
        AddressDecoder(){};

        AddressParts DecodeAddress(std::uint32_t address, std::int32_t block_size, std::int32_t index_bits, std::int32_t offset_bits );
       
};


#endif