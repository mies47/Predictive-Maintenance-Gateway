#include "xbee.h"

XbeeDestAddress::XbeeDestAddress(
    uint64_t xbeeDes64BitAddress,
    uint16_t xbeeDes16BitAddress)
{
    _xbeeDes64BitAddress = xbeeDes64BitAddress;
    _xbeeDest16BitAddress = xbeeDes16BitAddress;   
}
void XbeeDestAddress::getXbeeDest64BitAddress(uint8_t* addressBuffer64Bit)
{
    for(int i = 0; i < sizeof(uint64_t); i++) {
        addressBuffer64Bit[i] = _xbeeDes64BitAddress >> (8 * i);
    }
}
void XbeeDestAddress::getXbeeDest16BitAddress(uint8_t* addressBuffer16Bit)
{
    for(int i = 0; i < sizeof(uint16_t); i++) {
        addressBuffer16Bit[i] = _xbeeDest16BitAddress >> (8 * i);
    }
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
void XbeeRequest::constructFrame(uint8_t* frameBuffer)
{
    const int fixedFrameLength = 17; // Counting delimeter, length, address, checksum, etc.
    const int dataLength = sizeof(_data);
    const int frameLength = fixedFrameLength + dataLength;
    int currentIndex = 0;
    uint8_t checksum = 0x00;
    // Begin constructing the frame

    // Start delimeter
    frameBuffer[currentIndex] = START_DELIMITER;
    currentIndex++;

    // Length
    frameBuffer[currentIndex] = (uint8_t) frameLength >> 8; //MSB
    currentIndex++;
    frameBuffer[currentIndex] = (uint8_t) frameLength; //LSB
    currentIndex++;

    // Frame Type
    // Start Calculating checksum from here
    frameBuffer[currentIndex] = Tx_REQ_API_ID;
    checksum += Tx_REQ_API_ID;
    currentIndex++;

    // Frame ID - Do not set to 0. Setting Frame ID to '0' will disable response frame.
    frameBuffer[currentIndex] = _frameId;
    checksum += _frameId;
    currentIndex++;

    // Destination 64 bit address. Start from MSB
    uint8_t addressBuffer64Bit[sizeof(uint64_t)];
    _destinationAddress.getXbeeDest64BitAddress(addressBuffer64Bit);
    for(int i = sizeof(uint64_t); i > 0; i--) { // MSB First
        frameBuffer[currentIndex] = addressBuffer64Bit[i];
        checksum += addressBuffer64Bit[i];
        currentIndex++;
    }

    // Destination 16 bit address. Start from MSB
    uint8_t addressBuffer16Bit[sizeof(uint16_t)];
    _destinationAddress.getXbeeDest16BitAddress(addressBuffer16Bit);
    for(int i = sizeof(uint16_t); i > 0; i--) { // MSB First
        frameBuffer[currentIndex] = addressBuffer16Bit[i];
        checksum += addressBuffer16Bit[i];
        currentIndex++;
    }

    // Broadcast Radius
    frameBuffer[currentIndex] = _broadcastRadius;
    checksum += _broadcastRadius;
    currentIndex++;

    // Options
    frameBuffer[currentIndex] = _options;
    checksum += _options;
    currentIndex++;

    // RF Data
    for(int i = 0; i < sizeof(_data); i++) { // MSB first
        frameBuffer[currentIndex] = _data[i];
        checksum += _data[i];
        currentIndex++;
    }

    // Checksum
    checksum = 0xFF - checksum;
    frameBuffer[currentIndex] = checksum;
}
void XbeeRequest::writeFrameToSerial(uint8_t* frame, SoftwareSerial serialToWrite)
{
    serialToWrite.write(frame, sizeof(frame));
}