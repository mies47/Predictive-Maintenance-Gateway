import threading

from typing import List, Dict
from utils import get_current_time, update_cached_data
from env_vars import SAMPLING_FREQ, SAMPLING_WAIT_TIME

class VibrationData:
	def __init__(self, x: float, y: float, z: float):
		self.x = x
		self.y = y
		self.z = z


class Measurement:
	def __init__(self, id: str, data: List[VibrationData] = []):
		self.time = get_current_time()
		self.id = measurement_id
		self._data = measured_data

	def thread_handler(self, data_dict: Dict[str, Node]):
		'Completes the measurement data in case of frame loss then updates the cache'
		if len(self._data) < SAMPLING_FREQ:
			data_list_length = len(self._data)
			for i in range(SAMPLING_FREQ - data_list_length):
				self.add_new_data(self._data[i])
		
		update_cached_data(data_dict)

	def add_new_data(self, new_data: VibrationData):
		self._data.append(new_data)
	
	def add_new_data_list(self, new_data_list: List[VibrationData]):
		self._data.extend(new_data_list)


class Node:
	def __init__(self, node_id:str, measurements: Dict[str, Measurement] = {}):
		self.node_id = node_id
		self._measurements = measurements

	def add_new_measurement(self, measurement: Measurement, data_dict = Dict[str, Node]):
		'If this is a new measurement add it to node and start the timer to check sampling'
		if not self._measurements.get(measurement.id):
			self._measurements[measurement.id] = measurement
			threading.Timer(SAMPLING_WAIT_TIME, measurement.thread_handler, args=(data_dict,))