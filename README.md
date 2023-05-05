# Gateway

This repo contains all the codes for implementing a gateway to communicate between end nodes and predicitve maintenance server.

## Table of Contents

* [How to run gateway](#StartUp)

## StartUp
First pull the project to your local machine and navigate to the root directory of the project:
```cd Predictive-Maintenance-Gateway```

Add .env file based on the fields declared in .env.example file:
```cp .env.example .env```

Install all the required python packages using:
```pip3 install -r requirements.txt```

## Include Xbee in Arduino IDE

To use Xbee in Arduino IDE copy and paste the Xbee folder in ```${arduino_path}/libraries```. Open the IDE and either include it by hand or use Sketch > Include Library > Xbee.
