#!/bin/bash

umask 2

while true 
do
        
  sudo python /home/ubuntu/python_mqtt_sub_mongodb.py
          
  echo "[IoT dashsoard] Press [CTRL+C] to stop.."
              
  sleep 1

done