import uuid
import json
import datetime
import pickle

from typing import Dict, Tuple, List
from models import Node, VibrationData


class ModelJsonObject(json.JSONEncoder):
	def default(self, obj):
		if hasattr(obj, "__dict__"):
			return {key:value for key, value in obj.__dict__.items() if not key.startswith("_")}
		return super().default(obj)

 
def get_mac_address():
	return ':'.join(['{:02x}'.format((uuid.getnode() >> ele) & 0xff)
for ele in range(0,8*6,8)][::-1])


def get_current_time():
	current_datetime = datetime.datetime.now()
	current_datetime = current_datetime.astimezone(datetime.timezone.utc)
	current_datetime = current_datetime.replace(tzinfo=datetime.timezone.utc).timestamp()
	return current_datetime


def update_cached_data(data_dict: Dict[str, Node]):
    with open(f'{PWD}/cached', 'wb') as f:
        pickle.dump(data_list, f, protocol=pickle.HIGHEST_PROTOCOL)


def decode_sensor_data(raw_data: str) -> Tuple(str, List[VibrationData]):
	'Receives raw sensor data returns measurement_id, List[VibrationData] sensor_data'
	measurement_id = str(raw_data[0])
	measurement_data = []

	index = 1
	while index < len(raw_data):
		vibration_data = []
		for _ in range(3):
			vibration_data.append(raw_data[index:index+2] + '.' + raw_data[index+2:index+5])
			index += 5
		measurement_data.append(VibrationData(*vibration_data))

	
	return measurement_id, measurement_data