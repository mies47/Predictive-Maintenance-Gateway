#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <ArduinoJson.h>

const int SAMPLING_FREQ = 25;
const int SAMPLING_DELAY = 1000 / SAMPLING_FREQ;
/* Used ArduinoJson assistant at https://arduinojson.org/v6/assistant/ */
const long STRUCTURE_SIZE = 32;
const size_t MEASUREMENT_CAPACITY = SAMPLING_FREQ * STRUCTURE_SIZE;

/* Serial used for xbee communication */
SoftwareSerial xbeeSerial(1,0); //RX, TX

/* Sensor data for each round of measurement in 3 dminesions */
StaticJsonDocument<MEASUREMENT_CAPACITY> measurement;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
    xbeeSerial.begin(9600);
    Serial.begin(9600);

    /* Initialise the sensor */
    if(!accel.begin())
    {
        /* There was a problem detecting the ADXL345 ... check your connections */
        Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
        while(1);
    }

    /* Set the range from 2_G to 16_G the smaller the range more precise it gets */
    accel.setRange(ADXL345_RANGE_8_G);

    /* Set Sensor data rate. Should look out for device communication capability.*/
    accel.setDataRate(ADXL345_DATARATE_50_HZ);

}

void loop() {
    /* Create a new JSON array for each measurement */
    JsonArray arr = measurement.to<JsonArray>();

    for(int i = 0; i < SAMPLING_FREQ; i++) {
        /* Get a new sensor event */ 
        sensors_event_t event; 
        accel.getEvent(&event);
        
        JsonObject doc_0 = measurement.createNestedObject();
        doc_0["x"] = event.acceleration.x;
        doc_0["y"] = event.acceleration.y;
        doc_0["z"] = event.acceleration.z;

        delay(SAMPLING_DELAY);
    }

    /* Serialize and print the output to xbee */
    serializeJson(measurement, xbeeSerial);
    
    /* Delay per each round of measurement*/
    delay(1000);
}