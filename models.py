from typing import List
from utils import get_current_time

class VibrationData:
	def __init__(self, x: float, y: float, z: float):
		self.time = get_current_time()
		self.x = x
		self.y = y
		self.z = z


class DataModel:
	def __init__(self, nodeId: str, vibrationData: List[VibrationData] = []):
		self.nodeId = nodeId
		self.vibrationData = vibrationData

	def add_vibration_data(self, d: VibrationData):
		self.vibrationData.append(d)


class DataModelDict:
	def __init__(self, data: dict[str, DataModel] = {}):
		self.data = data

	def add_data_model(self, n_id: str, d: DataModel):
		if self.data.get(n_id):
			self.data[n_id].add_vibration_data(d)
		else:
			self.data[n_id] = DataModel(nodeId=n_id, vibrationData=d)
