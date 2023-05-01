import json
import requests

from utils import get_mac_address
from env_vars import REQUEST_PROTOCOL, SERVER_IP, SERVER_PORT, \
					 API_PREFIX, SIGN_UP_ENDPOINT, PASSWORD, PWD


BASE_URL = f'{REQUEST_PROTOCOL}://{SERVER_IP}:{SERVER_PORT}'


if __name__ == '__main__':
    sign_up_url = f'{BASE_URL}{API_PREFIX}{SIGN_UP_ENDPOINT}'

    body = {
		'mac': get_mac_address(),
		'password': PASSWORD
	}
    
    r = requests.post(sign_up_url, data=json.dumps(body))
    r.close()