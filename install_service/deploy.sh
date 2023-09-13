# deployment script for serial2mqtt run as a service in background always
sudo cp serial2mqtt.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl restart serial2mqtt
sudo systemctl status serial2mqtt

