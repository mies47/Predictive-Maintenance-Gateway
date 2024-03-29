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

Start the cached vibration data post cronjob and enable receive_data script as a startup service using:
```bash schedule.sh```

Start the receive data listener using:
```python3 receive_data.py```

## Include Xbee in Arduino IDE

To use Xbee in Arduino IDE copy and paste the Xbee folder in ```${arduino_path}/libraries```. Open the IDE and either include it by hand or use Sketch > Include Library > Xbee.
