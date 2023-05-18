/**
 * @file xbee.h
 * @author Milad Esrafilian
 * @brief 
 *  This library is written to provide the functionality of xbee communication
 *  in API mode. Other libraries including xbee-arduino have issues doing so.
 *  I suppose this comes from the miscalculation in checksum and the method of
 *  writing to the Serial port which is done byte by byte instead of writing the
 *  frame all together. The materials used to write this are publicly available
 *  at https://www.digi.com/resources/documentation/Digidocs/90001942-13/containers/cont_api_mode.htm
 * @version 0.1
 * @date 2023-04-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#ifndef Xbee_h
#define Xbee_h

#include "stdint.h"
#include "stdlib.h"
#include "SoftwareSerial.h"

#define START_DELIMITER 0x7E
#define Tx_REQ_API_ID 0x10
#define Tx_STATUS_API_ID 0x8B
#define FIXED_TX_FRAME_LENGTH 18
#define MAX_SAMPLE_PER_FRAME 15


class XbeeDestAddress
{
    public:
        XbeeDestAddress(uint64_t xbeeDes64BitAddress, uint16_t xbeeDes16BitAddress);
        void getXbeeDest64BitAddress(uint8_t* addressBuffer64Bit);
        void getXbeeDest16BitAddress(uint8_t* addressBuffer16Bit);
    private:
        uint64_t _xbeeDes64BitAddress;
        uint16_t _xbeeDest16BitAddress;

};

class XbeeRequest
{
    public:
        XbeeRequest(
            uint8_t frameType,
            XbeeDestAddress destinationAddress,
            uint8_t broadcastRadius,
            uint8_t options
        );
        void constructFrame(
            uint8_t frameId, 
            uint8_t* frameBuffer, 
            size_t bufferSize, 
            float* data, 
            int dataStartIndex,
            int dataEndIndex
        );
        void writeFrameToSerial(
            uint8_t* frame, 
            size_t frameSize, 
            SoftwareSerial serialToWrite
        );
    private:
        uint8_t _frameType;
        XbeeDestAddress _destinationAddress;
        uint8_t _broadcastRadius; // If set to '0', the broadcast radius will be set to the maximum hops value.
        uint8_t _options; // No option is actually needed. Set to 0

};

#endif