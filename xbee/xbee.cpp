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


XbeeRequest::XbeeRequest(
    uint8_t frameType,
    uint8_t frameId,
    XbeeDestAddress destinationAddress,
    uint8_t broadcastRadius,
    uint8_t options,
    uint8_t* data): _destinationAddress(destinationAddress)
{
    uint8_t _frameType = frameType;
    uint8_t _frameId = frameId; // Do not set to 0. Setting Frame ID to '0' will disable response frame.
    uint8_t _broadcastRadius = broadcastRadius; // If set to '0', the broadcast radius will be set to the maximum hops value.
    uint8_t _options = options; // No option is actually needed. Set to 0
    uint8_t* _data = data;
}
uint8_t* XbeeRequest::constructFrame()
{
    const int fixedFrameLength = 17; // Counting delimeter, length, address, checksum, etc.
    const int dataLength = sizeof(_data);
    const int frameLength = fixedFrameLength + dataLength;
    uint8_t frame[frameLength];
    int currentIndex = 0;
    uint8_t checksum = 0x00;
    // Begin constructing the frame

    // Start delimeter
    frame[currentIndex] = START_DELIMITER;
    currentIndex++;

    // Length
    frame[currentIndex] = (uint8_t) frameLength >> 8; //MSB
    currentIndex++;
    frame[currentIndex] = (uint8_t) frameLength; //LSB
    currentIndex++;

    // Frame Type
    // Start Calculating checksum from here
    frame[currentIndex] = Tx_REQ_API_ID;
    checksum += Tx_REQ_API_ID;
    currentIndex++;

    // Frame ID - Do not set to 0. Setting Frame ID to '0' will disable response frame.
    frame[currentIndex] = _frameId;
    checksum += _frameId;
    currentIndex++;

    // Destination 64 bit address. Start from MSB


    

}