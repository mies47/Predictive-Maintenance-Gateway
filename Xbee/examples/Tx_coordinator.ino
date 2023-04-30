
#include <SoftwareSerial.h>
#include <Xbee.h>

/*
In this example a Xbee device connected to an arduino via serial ports
sends "Hello" message to the coordinator every 3 seconds using Xbee lib.
*/

SoftwareSerial abc(1, 0); //RX, TX
// These are the correct addresses for a coordinator
XbeeDestAddress xbeeDestinationAddress = XbeeDestAddress(0x00000000, 0xFFFE);
uint8_t data[] = {'H', 'e', 'l', 'l', 'o'};
XbeeRequest xbeeReq = XbeeRequest(Tx_REQ_API_ID, xbeeDestinationAddress, 0x00, 0x00);
uint8_t frame[FIXED_TX_FRAME_LENGTH + sizeof(data)];

void setup() {
  // put your setup code here, to run once:
  abc.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  xbeeReq.constructFrame(0x01, frame, sizeof(frame), data, sizeof(data));

  xbeeReq.writeFrameToSerial(frame, sizeof(frame), abc);

  delay(3000);
}