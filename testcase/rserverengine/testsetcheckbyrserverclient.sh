#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

mosquitto_pub -t "rserver\qrserver\qregister\put" -f qrserver/qregister/put.json
sleep 2
mosquitto_pub -t "rserver\qrserverclient\command"  -f qrserverclient/check.json