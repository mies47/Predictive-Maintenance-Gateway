from typing import List
from utils import get_current_time

class VibrationData:
	def __init__(self, measurmentId: str, x: float, y: float, z: float):
		self.time = get_current_time()
		self.measurementId = measurmentId
		self.x = x
		self.y = y
		self.z = z


class DataModel:
	def __init__(self, nodeId: str, vibrationData: List[VibrationData] = []):
		self.nodeId = nodeId
		self.vibrationData = vibrationData

	def add_vibration_data(self, d: VibrationData):
		self.vibrationData.append(d)


class DataModelList:
	def __init__(self, data: List[DataModel] = []):
		self.data = data

	def add_data_model(self, d: DataModel):
		self.data.append(d)
