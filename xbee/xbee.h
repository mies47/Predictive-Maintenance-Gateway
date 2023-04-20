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

#include "Arduino.h"

#define START_DELIMITER 0x7E;
#define Tx_REQ_API_ID 0x10;
#define Tx_STATUS_API_ID 0x8B;
// #define BROADCAST_RADIUS 0x00; // If set to '0', the broadcast radius will be set to the maximum hops value.
// #define OPTIONS 0x00; // No option is actually needed

class XbeeDestAddress
{
    public:
        XbeeDestAddress(uint64_t xbeeDes64BitAddress, uint16_t xbeeDes16BitAddress);
        uint8_t* getXbeeDest64BitAddress();
        uint8_t* getXbeeDest16BitAddress();
    private:
        uint64_t _xbeeDes64BitAddress;
        uint16_t _xbeeDest16BitAddress;

}

class XbeeRequestFrame
{
    public:
        XbeeRequestFrame(
            uint8_t frameType,
            uint8_t frameId,
            XbeeDestAddress destinationAddress,
            uint8_t broadcastRadius,
            uint8_t options,
            uint8_t* data
        );
        uint8_t* constructFrame();
        void writeFrameToSerial();
    private:
        uint8_t _frameType;
        uint8_t _frameId; // Do not set to 0. Setting Frame ID to '0' will disable response frame.
        XbeeDestAddress _destinationAddress;
        uint8_t _broadcastRadius; // If set to '0', the broadcast radius will be set to the maximum hops value.
        uint8_t _options; // No option is actually needed. Set to 0
        uint8_t* _data; // Up to 255 bytes of data could be sent.

}

#end_if