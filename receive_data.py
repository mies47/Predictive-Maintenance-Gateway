import json
import uuid
import pickle
import logging

from digi.xbee.devices import XBeeDevice

from models import VibrationData, DataModelList
from env_vars import XBEE_DIR, BAUD_RATE, PWD


logger = logging.basicConfig(filename="reciever.log")


def update_cached_data(data_list: DataModelList):
    with open(f'{PWD}/cached', 'wb') as f:
        pickle.dump(data_list, f, protocol=pickle.HIGHEST_PROTOCOL)


def fill_vibration(raw_data: str, measurement_id: str) -> VibrationData:
    decoded_data = json.loads(raw_data)
    return VibrationData(measurementId=measurement_id, x=decoded_data.x, y=decoded_data.y, z=decoded_data.z)


def recieve(data_list: DataModelList, measurement_id: str):
    device = XBeeDevice(XBEE_DIR, BAUD_RATE)

    try:
        device.open()

        device.flush_queues()

        while True:
            xbee_message = device.read_data()

            if xbee_message is not None:
                node_id = xbee_message.remote_device.get_64bit_addr()
                vibration_data = fill_vibration(raw_data=xbee_message.data, measurement_id=measurement_id)
                
                data_list.add_vibration_data(nodeId=node_id, vibrationData=vibration_data)
                update_cached_data(data_list)
    
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
			return DataModelList()


if __name__ == '__main__':
    data_list = load_cached_data()

    measurement_id = uuid.uuid4().hex

    while True:
        recieve(data_list=data_list, measurement_id=measurement_id)