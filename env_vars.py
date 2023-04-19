import os

from dotenv import load_dotenv


load_dotenv()


REQUEST_PROTOCOL = os.getenv('REQUEST_PROTOCOL')
SERVER_IP = os.getenv('SERVER_IP')
SERVER_PORT = os.getenv('SERVER_PORT')
API_PREFIX = os.getenv('API_PREFIX')
DATA_ENDPOINT = os.getenv('DATA_ENDPOINT')
TOKEN_ENDPOINT = os.getenv('TOKEN_ENDPOINT')


XBEE_DIR = os.getenv('XBEE_DIR')
BAUD_RATE = os.getenv('BAUD_RATE')

PASSWORD = os.getenv('PASSWORD')