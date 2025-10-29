#! /usr/bin/bash

TIME=$(date +%Y%m%d%H%M)
echo "Test time: $TIME"
LOG_DIR=./logs
LOG_FILE=$LOG_DIR/test_$TIME.log
mkdir -p $LOG_DIR
echo "Test log file: $LOG_FILE"
"$@" 2>&1 | tee $LOG_FILE
echo "Written Test log file: $LOG_FILE"