# pGRAMSFC (pGRAMS Flight Computer)

Software for pGRAMS flight computer

## Author

- Shota Arai

## Aims

This repository is intended for the following usage:

- Collect data from housekeeping (HK) sensors
- Create packets and send them to the ground
- Send commands from the ground
- Interpret the received packets to push to MySQL database for Grafana displaying

## Structure

pGramsFC<br>
|--[onboard](onboard):  Onboard software and ground receiver <br>
|--[command_sender](command_sender):  Command sender software <br>
|--[operation](operation):   Ruby scripts to execute the software<br>
|--[examples](examples):   Example code for unit tests<br>
|--[service](service):  Scripts to create deamon for onboard/ground software<br>
|--[settings](settings): Example of setting files<br>
|--[tools](tools): Useful tools and scripts for basic tests<br>

## Environment

- MacOS Tahoe
- Ubuntu 20.04

## Installation

Please look at [this page](docs/Installation.md)
