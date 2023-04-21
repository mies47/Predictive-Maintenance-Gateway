#include "xbee.h"

XbeeDestAddress::XbeeDestAddress(
    uint64_t xbeeDes64BitAddress,
    uint16_t xbeeDes16BitAddress)
{
    _xbeeDes64BitAddress = xbeeDes64BitAddress;
    _xbeeDest16BitAddress = xbeeDes16BitAddress;   
}

uint8_t* XbeeDestAddress::getXbeeDest64BitAddress()
{
    uint8_t destAddressByteArray[sizeof(uint64_t)];
    for(int i = 0; i < sizeof(uint64_t); i++) {
        destAddressByteArray[i] = _xbeeDes64BitAddress >> (8 * i);
    }
    return destAddressByteArray;
}

uint8_t* XbeeDestAddress::getXbeeDest16BitAddress()
{
    uint8_t destAddressByteArray[sizeof(uint16_t)];
    for(int i = 0; i < sizeof(uint16_t); i++) {
        destAddressByteArray[i] = _xbeeDest16BitAddress >> (8 * i);
    }
    return destAddressByteArray;
}