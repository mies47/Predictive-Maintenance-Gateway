cwd=$(pwd | sed 's/ /\\ /g')
crontab -u $USER -r
(crontab -u $USER -l; echo "*/15 * * * * /usr/bin/python3 $cwd/send_data.py") | crontab -