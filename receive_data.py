import pickle
import logging

from digi.xbee.devices import XBeeDevice
from typing import Dict, Tuple, List

from models import Node, VibrationData
from env_vars import XBEE_DIR, BAUD_RATE, PWD


logger = logging.basicConfig(filename="reciever.log")


def decode_sensor_data(raw_data: str) -> Tuple[str, List[VibrationData]]:
    'Receives raw sensor data returns measurement_id, List[VibrationData] sensor_data'
    measurement_id = str(raw_data[0])
    measurement_data = []

    index = 1
    while index < len(raw_data):
        vibration_data = []
        for _ in range(3):
            vibration_data.append(
                float(raw_data[index:index+2].decode('utf-8') + '.' + raw_data[index+2:index+5].decode('utf-8'))
            )
            index += 5
        measurement_data.append(VibrationData(*vibration_data))

    return measurement_id, measurement_data
    

def recieve(data_dict: Dict[str, Node]):
    device = XBeeDevice(XBEE_DIR, BAUD_RATE)

    try:
        device.open()

        device.flush_queues()

        while True:
            xbee_message = device.read_data()

            if xbee_message is not None:
                node_id = str(xbee_message.remote_device.get_64bit_addr())
                measurement_id, measurement_data = decode_sensor_data(xbee_message.data)

                # Update the data_dict if the data is sent
                data_dict = load_cached_data()

                if not data_dict.get(node_id):
                    data_dict[node_id] = Node(node_id)
                
                data_dict[node_id].add_measurement(
                    measurement_id,
                    measurement_data,
                    data_dict
                )
    
    except Exception as e:
        logging.exception(e)

    finally:
        if device is not None and device.is_open():
            device.close()


def load_cached_data():
    try:
        with open(f'{PWD}/cached', 'rb') as f:
            data = pickle.load(f)
            return data
    except:
        return {}


if __name__ == '__main__':
    data_dict = load_cached_data()

    while True:
        recieve(data_dict=data_dict)
