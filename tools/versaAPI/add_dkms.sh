#! /usr/bin/env bash

DESTINATION="/usr/src/vl_driver-1.8.4"

sudo mkdir $DESTINATION
set -euo pipefail 

sudo cp dkms-build.sh $DESTINATION
sudo cp dkms.conf $DESTINATION
sudo cp -r src $DESTINATION
sudo dkms remove -m vl_driver/1.8.4 --all || true
sudo dkms add -m vl_driver/1.8.4
sudo dkms build -m vl_driver/1.8.4
sudo dkms install -m vl_driver/1.8.4
