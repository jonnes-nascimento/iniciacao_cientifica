#!/bin/bash
sudo systemctl start mosquitto.service
sudo systemctl start iot_dashboard.service
sudo python ~/iot_dashboard/iot_dashboard.py &
