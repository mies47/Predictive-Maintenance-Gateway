import threading

from typing import List, Dict
from utils import get_current_time, update_cached_data
from env_vars import SAMPLING_FREQ, SAMPLING_WAIT_TIME

class VibrationData:
	def __init__(self, x: float, y: float, z: float):
		self.x = x
		self.y = y
		self.z = z
	
	def __repr__(self) -> str:
		return f"x: {self.x}, y: {self.y}, z: {self.z}"


class Measurement:
	def __init__(self, id: str, data: List[VibrationData] = []):
		self.time = get_current_time()
		self.id = id
		self.data = data

	def thread_handler(self, data_dict):
		'Completes the measurement data in case of frame loss then updates the cache'
		if len(self.data) < SAMPLING_FREQ:
			data_list_length = len(self.data)
			for i in range(SAMPLING_FREQ - data_list_length):
				self.add_new_data(self.data[i])
		
		update_cached_data(data_dict)

	def add_new_data(self, new_data: VibrationData):
		self.data.append(new_data)
	
	def add_new_data_list(self, new_data_list: List[VibrationData]):
		self.data.extend(new_data_list)


class Node:
	def __init__(self, node_id:str, measurements: Dict[str, Measurement] = {}):
		self.node_id = node_id
		self.measurements = measurements

	def add_measurement(
		self,
		id: str,
		data: List[VibrationData],
		data_dict
	):
		'''
		If this is a new measurement add it to node and start the timer to check
		sampling, if not, add it to the previously added measurement.
		'''
		if not self.measurements.get(id):
			self.measurements[id] = Measurement(id, data)
			threading.Timer(
				SAMPLING_WAIT_TIME,
				self.measurements[id].thread_handler,
				args=(data_dict,)
			).start()
		else:
			self.measurements[id].add_new_data_list(data)