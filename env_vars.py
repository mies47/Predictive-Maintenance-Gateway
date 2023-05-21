import os

from dotenv import load_dotenv


load_dotenv()


PWD = os.getenv('PWD').replace(' ', '\ ')
REQUEST_PROTOCOL = os.getenv('REQUEST_PROTOCOL')
SERVER_IP = os.getenv('SERVER_IP')
SERVER_PORT = os.getenv('SERVER_PORT')
API_PREFIX = os.getenv('API_PREFIX')
DATA_ENDPOINT = os.getenv('DATA_ENDPOINT')
TOKEN_ENDPOINT = os.getenv('TOKEN_ENDPOINT')
SIGN_UP_ENDPOINT = os.getenv('SIGN_UP_ENDPOINT')


SAMPLING_FREQ = int(os.getenv('SAMPLING_FREQ'))
SAMPLING_WAIT_TIME = int(os.getenv('SAMPLING_WAIT_TIME'))

XBEE_DIR = os.getenv('XBEE_DIR')
BAUD_RATE = int(os.getenv('BAUD_RATE'))

PASSWORD = os.getenv('PASSWORD')