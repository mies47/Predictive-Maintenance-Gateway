import sys
import json
import pickle
import requests

from models import VibrationData, DataModel, DataModelList
from utils import get_current_time, get_mac_address, ModelJsonObject
from env_vars import REQUEST_PROTOCOL, SERVER_IP, SERVER_PORT, API_PREFIX, DATA_ENDPOINT


# with open('cached', 'rb') as f:
#	data = pickle.load(f)


# TODO: Check if a specific amount of time has passed since the last data transfer
if True:
	baseUrl = f'{REQUEST_PROTOCOL}://{SERVER_IP}:{SERVER_PORT}'
	url = f'{baseUrl}{API_PREFIX}{DATA_ENDPOINT}'
	obj = VibrationData(measurmentId='ThisIsATest', x=2, y=3.0, z=4.0)
	n_id = get_mac_address()
	dataModel = DataModel(n_id)
	dataModel.add_vibration_data(obj)
	d = DataModelList()
	d.add_data_model(dataModel)
	print(url, json.dumps(d, cls=ModelJsonObject, indent=4))
	r = requests.post(url, data=json.dumps(d, cls=ModelJsonObject, indent=4))
	# TODO: Check if data is sent successfully
	with open('cached', 'wb') as f:
		pass
