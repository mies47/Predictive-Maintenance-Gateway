#include "Xbee.h"
#include "Arduino.h"


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

/* Utill function to convert float to a uint8_t* format ex. 3.345 -> {"+", "3", "3", "4", "5"} */
void saveFloatInBuffer(float number, uint8_t* buffer, size_t bufferSize);


XbeeRequest::XbeeRequest(
    uint8_t frameType,
    XbeeDestAddress destinationAddress,
    uint8_t broadcastRadius,
    uint8_t options): _destinationAddress(destinationAddress)
{
    uint8_t _frameType = frameType;
    // If set to '0', the broadcast radius will be set to the maximum hops value.
    uint8_t _broadcastRadius = broadcastRadius;
    // No option is actually needed. Set to 0
    uint8_t _options = options;
}
void XbeeRequest::constructFrame(
    uint8_t frameId, 
    uint8_t* frameBuffer, 
    size_t bufferSize, 
    uint8_t* data, 
    size_t dataSize)
{
    // Exclude start delimeter, length, and checksum
    const int frameLength = bufferSize - 4;
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
    frameBuffer[currentIndex] = frameId;
    checksum += frameId;
    currentIndex++;

    // Destination 64 bit address. Start from MSB
    uint8_t addressBuffer64Bit[sizeof(uint64_t)];
    _destinationAddress.getXbeeDest64BitAddress(addressBuffer64Bit);
    for(int i = sizeof(uint64_t) - 1; i >= 0; i--) { // MSB First
        frameBuffer[currentIndex] = addressBuffer64Bit[i];
        checksum += addressBuffer64Bit[i];
        currentIndex++;
    }

    // Destination 16 bit address. Start from MSB
    uint8_t addressBuffer16Bit[sizeof(uint16_t)];
    _destinationAddress.getXbeeDest16BitAddress(addressBuffer16Bit);
    for(int i = sizeof(uint16_t) - 1; i >= 0; i--) { // MSB First
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

    /*  
        First byte of every frame payload is frameID since frameID assigned in
        Tx frame is not received in Rx frame.
    */
    frameBuffer[measurementIndex] = (uint8_t) data[0];
    measurementIndex++;

    // Not a very clean implementation!
    // But hey, I'm up against many constraints and my hands are tied.
    for(int i = 1; i < dataSize; i++) { // MSB first
        saveFloatInBuffer(data[i], frameBuffer, currentIndex);
        for(int j = 0; j < 5; j++) {
            checksum += frameBuffer[currentIndex];
            currentIndex++;
        }
    }

    // Checksum
    checksum = 0xFF - checksum;
    frameBuffer[currentIndex] = checksum;
}
void XbeeRequest::writeFrameToSerial(
    uint8_t* frame, 
    size_t frameSize, 
    SoftwareSerial serialToWrite)
{
    serialToWrite.write(frame, frameSize);
}


void saveFloatInBuffer(float number, uint8_t* buffer, int buffIndex) {
    if(number > 0) {
        buffer[buffIndex] = (uint8_t) '+';
    } else if (number < 0) {
        buffer[buffIndex] = (uint8_t) '-';
        number *= -1;
    } else {
        buffer[buffIndex] = '0';
    }

    buffer[buffIndex + 1] = ((int)number % 10) + '0';
    
    for(int i = 1; i <= 3; i++) {
        buffer[buffIndex+i+1] = ((int)(number*(10^i)) % 10) + '0';
    }
    
}