import uuid
import json
import datetime
import pickle

from env_vars import PWD

class ModelJsonObject(json.JSONEncoder):
	def default(self, obj):
		if hasattr(obj, "__dict__"):
			return {key:value for key, value in obj.__dict__.items() if not key.startswith("_")}
		return super().default(obj)

 
def get_mac_address():
	return ':'.join(['{:02x}'.format((uuid.getnode() >> ele) & 0xff)
for ele in range(0,8*6,8)][::-1])


def get_current_time():
	current_datetime = datetime.datetime.now()
	current_datetime = current_datetime.astimezone(datetime.timezone.utc)
	current_datetime = current_datetime.replace(tzinfo=datetime.timezone.utc).timestamp()
	return current_datetime


def update_cached_data(data_dict):
    with open(f'{PWD}/cached', 'wb') as f:
        pickle.dump(data_dict, f, protocol=pickle.HIGHEST_PROTOCOL)