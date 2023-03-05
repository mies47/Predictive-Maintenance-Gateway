import logging
import json
from digi.xbee.devices import XBeeDevice
from models import VibrationData, DataModel, DataModelDict

logger = logging.basicConfig(filename="reciever.log")

PORT = "/dev/ttyUSB0"
BAUD_RATE = 9600

def fill_vibration(raw_data: str) -> VibrationData:
    decoded_data = json.loads(raw_data)
    return VibrationData(x=decoded_data.x, y=decoded_data.y, z=decoded_data.z)

def recieve(data_dict: DataModelDict):
    device = XBeeDevice(PORT, BAUD_RATE)

    try:
        device.open()

        device.flush_queues()

        while True:
            xbee_message = device.read_data()

            if xbee_message is not None:
                node_id = xbee_message.remote_device.get_64bit_addr()
                vibration_data = fill_vibration(xbee_message.data)
                
                data_dict.add_data_model(node_id, DataModel(node_id, vibration_data))
    except Exception as e:
        logging.exception(e)
    finally:
        if device is not None and device.is_open():
            device.close()


if __name__ == '__main__':
    data_dict = DataModelDict()

    while True:
        recieve(data_dict)
