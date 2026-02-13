#! /bin/sh

SCRIPT_DIR="$(cd $(dirname $0); pwd)"
HOME_DIR=`getent passwd ${SUDO_USER:-$USER} | cut -d: -f6`
EXE_DIR=$SCRIPT_DIR/../operation/neu_test_20260128
EXE_PATH=$EXE_DIR/neu_test_20260128.rb
INSTALL_PATH=/etc/pGRAMS
ENV_FILE_PATH=${INSTALL_PATH}/pGramsFC.env

. ${HOME_DIR}/pGRAMS.sh
retval=$?
if [ $retval -ne 0 ]; then
  exit $retval
fi

sudo mkdir ${INSTALL_PATH}
#retval=$?
#if [ $retval -ne 0 ]; then
#  exit $retval
#fi

sudo echo -e "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}\nRUBYLIB=${RUBYLIB}\nPGRAMS_MOSQUITTO_HOST=${PGRAMS_MOSQUITTO_HOST}\nPGRAMS_MOSQUITTO_PORT=${PGRAMS_MOSQUITTO_PORT}\nPGRAMS_MOSQUITTO_USER=${PGRAMS_MOSQUITTO_USER}\nPGRAMS_MOSQUITTO_PASSWD=${PGRAMS_MOSQUITTO_PASSWD}\nPGRAMS_EXE_DIR=${EXE_DIR}\nPGRAMS_SHELL_DIR=${SCRIPT_DIR}\nHOME=${HOME_DIR}" > $ENV_FILE_PATH
retval=$?
if [ $retval -ne 0 ]; then
  exit $retval
fi

sudo cp ${SCRIPT_DIR}/pGramsFC.service /etc/systemd/system
retval=$?
if [ $retval -ne 0 ]; then
  exit $retval
fi

sudo systemctl daemon-reload