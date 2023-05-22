import json
import pickle
import requests
from typing import Dict

from models import Node
from utils import get_mac_address, ModelJsonObject
from env_vars import REQUEST_PROTOCOL, SERVER_IP, SERVER_PORT, \
					 API_PREFIX, DATA_ENDPOINT, TOKEN_ENDPOINT, PASSWORD, PWD


TOKEN = ''
BASE_URL = f'{REQUEST_PROTOCOL}://{SERVER_IP}:{SERVER_PORT}'


def get_new_token() -> str:
	token_url = f'{BASE_URL}{API_PREFIX}{TOKEN_ENDPOINT}'

	body = {
		'mac': get_mac_address(),
		'password': PASSWORD
	}

	r = requests.post(token_url, data=json.dumps(body))
	r.close()

	return r.json().get('token')


def send_vibration_data(cached_data: Dict[str, Node]):
	global TOKEN

	if cached_data is None:
		return

	send_url = f'{BASE_URL}{API_PREFIX}{DATA_ENDPOINT}'

	auth_header = lambda token: {'Authorization': f'Bearer {token}'}

	r = requests.post(send_url, data=json.dumps(cached_data, cls=ModelJsonObject, indent=4), headers=auth_header(TOKEN))

	if r.status_code in [401, 403]:
		TOKEN = get_new_token()
		r = requests.post(send_url, data=json.dumps(cached_data, cls=ModelJsonObject, indent=4), headers=auth_header(TOKEN))
	
	r.close()

	if r.status_code == 202:
		open('cached', 'wb').close()


def load_cached_data():
		try:
			with open(f'{PWD}/cached', 'rb') as f:
				data = pickle.load(f)
				return data
		except:
			return None


if __name__ == '__main__':

	cached_data = load_cached_data()

	send_vibration_data(cached_data=cached_data)
