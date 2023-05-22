#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <Xbee.h>
#include <LowPower.h>

/*
    Sampling frequency could be as much as you'd like considering serial communication to
    Xbee module from arduino and also RF communication between two Xbee nodes.

    IMPORTANT: There is a maximum payload size for API mode communication. Thus, a fragmentation
    is needed for any payload > 255. A max sample per frame is defined to ensure this.
    Sampling_freq should be a multiple of 15 to avoid sending incorrect data since the array is
    statistically allocated.
*/
const int SAMPLING_FREQ = 60;
const int SAMPLING_DELAY = 1000 / SAMPLING_FREQ;
const int MEASUREMENT_SLEEP_PERIOD = 20 * 60 * 1000; // Every 20 minutes
const int FRAGMENTATION_DELAY = 3 * 1000; // 3 seconds delay to ensure correct frame delivery

/* Any measured number is represented in 5 bytes ex. -0.365 -> {'-', '0', '3', '6', '5'}*/
const long STRUCTURE_SIZE = 15;
const size_t MEASUREMENT_CAPACITY = MAX_SAMPLE_PER_FRAME * STRUCTURE_SIZE;
const long TOTAL_FRAME_SIZE = MEASUREMENT_CAPACITY + 18 + 1;

/* Serial used for xbee communication */
SoftwareSerial xbeeSerial(1,0); //RX, TX

/* Buffer to create Xbee frame in */
uint8_t frameBuffer[TOTAL_FRAME_SIZE];

/*
    Every sample is in x, y, z dimension.
    Save sensory float data in real time to ensure correct sampling rate.
    There is some MEMORY CONSTRAINTS when defining this array in Arduino boards.
    Try to stick to 60 or less sampling rate to avoid any stability problems.
*/
float data[SAMPLING_FREQ * 3];


/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// These are the correct addresses for a coordinator
XbeeDestAddress xbeeDestinationAddress = XbeeDestAddress(0x00000000, 0xFFFE);
XbeeRequest xbeeReq = XbeeRequest(Tx_REQ_API_ID, xbeeDestinationAddress, 0x00, 0x00);

/* 
    Function to sleep for a longer period. The max sleep period is 8 seconds.
    This method uses arduino watchdog timer. Watchdog is pretty inaccurate but does
    the job in our case. Other methods are using external interrupt or timer comp.
 */
void longSleep(int sleep_period);

void setup() {
    xbeeSerial.begin(9600);

    // Use to generate psuedo random numbers
    randomSeed(1);

    /* Initialise the sensor */
    if(!accel.begin())
    {
        /* There was a problem detecting the ADXL345 ... check your connections */
        while(1);
    }

    /* Set the range from 2_G to 16_G the smaller the range more precise it gets */
    accel.setRange(ADXL345_RANGE_2_G);

    /* Set Sensor data rate. Should look out for device communication capability.*/
    accel.setDataRate(ADXL345_DATARATE_100_HZ);
}

void loop() {
    /*
        First sample and then do the fragmentation.
        There is a cool down delay for each frame sent to ensure its delivery.
        Sampling while sending frames could lead to incorrect sampling due to
        unpredictable delays.
    */
    for(int i = 0; i < SAMPLING_FREQ * 3; i=i+3) {
        sensors_event_t event; 
        accel.getEvent(&event);

        data[i] = event.acceleration.x;
        data[i + 1] = event.acceleration.y;
        data[i + 2] = event.acceleration.z;

        delay(SAMPLING_DELAY);
    }

    /*  
        First byte of every frame payload is frameID since frameID assigned in
        Tx frame is not received in Rx frame.
    */
    uint8_t frameId = (uint8_t) random(1, (int) 0xFF);

    // Fragmentation
    for (int i = 0; i < (int)(SAMPLING_FREQ / MAX_SAMPLE_PER_FRAME); i++) {
        int dataStartIndex = i * 3 * MAX_SAMPLE_PER_FRAME;
        int dataEndIndex = ((i + 1) * 3 * MAX_SAMPLE_PER_FRAME) - 1;

        xbeeReq.constructFrame(
            frameId,
            frameBuffer,
            TOTAL_FRAME_SIZE,
            data,
            dataStartIndex,
            dataEndIndex
        );

        xbeeReq.writeFrameToSerial(frameBuffer, TOTAL_FRAME_SIZE, xbeeSerial);

        delay(FRAGMENTATION_DELAY);
    }

    /* PowerDown per each round of measurement to save energy */
    longSleep(MEASUREMENT_SLEEP_PERIOD);
}

void longSleep(int sleep_period) {
    while(sleep_time > 0) {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
        sleep_time -= 8000;
    } 
} 