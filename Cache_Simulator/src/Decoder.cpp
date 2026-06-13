#include "AddressParts.h"
#include "Decoder.h"

// inline namespace {
    AddressParts  AddressDecoder::DecodeAddress(std::uint32_t address, std::int32_t block_size, std::int32_t index_bits, std::int32_t offset_bits )
    {
        parts.offset = address & (block_size -1);
        parts.index = (address>> offset_bits) & ((1<<index_bits) -1);
        parts.tag = address >> (offset_bits + index_bits);
        return parts;
    }
    
// }