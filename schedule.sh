cwd=$(pwd | sed 's/ /\\ /g')

# Add cron job if not exists
(crontab -l | grep "/send_data.py") || (crontab -l; echo "*/15 * * * * ${USER} /usr/bin/python3 ${cwd}/send_data.py &") | crontab -

# Replace strings in example file and create the main service file
cat receiver.example.service | sed "s/USER_TO_REPLACE/${USER}/" | sed "s|PATH_TO_REPLACE|${cwd}/receive_data.py|" > receiver.service

# Move this file to /etc
sudo mv receiver.service /etc/systemd/system/

# Reload systemctl daemon to effect changes
sudo systemctl daemon-reload

# Enable this service startup
sudo systemctl enable receiver.service

echo "Starting Service"

sudo systemctl start receiver.service

sudo systemctl status receiver.service