import sys
import json
import pickle
import requests

from models import VibrationData, DataModel, DataModelList
from utils import get_current_time, get_mac_address, ModelJsonObject


with open('config.json') as f:
	config = json.load(f)

# with open('cached', 'rb') as f:
#	data = pickle.load(f)

REQUEST_PROTOCOL = config['REQUEST_PROTOCOL']
SERVER_IP = config['SERVER_IP']
SERVER_PORT = config['SERVER_PORT']
API_PREFIX = config['API_PREFIX']
DATA_THRESHOLD = config['DATA_THRESHOLD']

# TODO: Check cached data size based on threshold in config
if True:
	url = f'{REQUEST_PROTOCOL}://{SERVER_IP}:{SERVER_PORT}/{API_PREFIX}'
	url = f'{url}/data/'
	obj = VibrationData(2, 3.0, 4.0)
	p = DataModel(get_mac_address())
	p.add_vibration_data(obj)
	o = DataModelList()
	o.add_data_model(p)
	print(url, json.dumps(o, cls=ModelJsonObject, indent=4))
	r = requests.post(url, data=json.dumps(o, cls=ModelJsonObject, indent=4))
	# TODO: Check if data is sent successfully
	with open('cached', 'wb') as f:
		pass
