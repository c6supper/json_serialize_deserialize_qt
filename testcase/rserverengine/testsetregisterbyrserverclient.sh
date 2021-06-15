#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

mosquitto_pub -t "rserver\qrserver\qregister\command" -f qrserver/qregister/put.json