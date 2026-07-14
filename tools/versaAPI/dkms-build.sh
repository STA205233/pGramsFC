#!/usr/bin/env bash
set -euo pipefail
VERSION=1.8.4;
LIB_DIR=/usr/local/lib;
VL_LIB_NAME=libVL_OSALib.$VERSION.so;
CG_LIB_NAME=$VL_LIB_NAME;
MCU_LIB_NAME=libVL_OSALib_mcu.$VERSION.so;
ARM_LIB_NAME=libVL_OSALib_arm.$VERSION.so;
SRC_LIB_NAME="";
LIB_CONF=/etc/ld.so.conf;


printUsage ()
{
    echo "Usage: $0 <kernel_version> <board_name>";
    echo "    Where VERSALOGIC_BOARD_NAME must be one of the following:";
    echo "      EBX-18, EBX-38, EPM-31, EPM-43, EPMe-30, EPM-39, EPMe-42, EPMe-51, EPU-5120, "
    echo "      EPM-19, EPU-3311, EPU-3312, EPU-4x62, EPU-4460, EPU-4011, EPU-4012, EPU-5121, ";
    echo "      MPEe-A1/A2, MPEe-U2, ESU-5070";
    echo "    Compile the VersaLogic API drivers \"vldrive*\".";
    echo "    This script should be run as user root, or a user with similar access.";
}

echo "VerasLogic Corporation VersaAPI Library and Driver Installation script";
echo "***** VerasLogic Corporation VersaAPI Library Installation *****";
echo "Version:$VERSION";
echo `date`;
echo "";

LIB_CG="no";
LIB_CG_NAME="libcgos.so";
VL_CG_DEV_NAME="/dev/cgos";
VL_PERM_FILE="S04vl_check_drivers";

KVER="${1:?kernel version required}"     # DKMSが渡すカーネル版


BOARD_NAME="";
FPGA_ADDR="0xC80";

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# ---- vl_install.sh の board->FPGA_ADDR 対応（必要分だけ書けばOK）----
if [ $# -eq 2 ] 
then
    BOARD_NAME=$2;
    echo "Board: $BOARD_NAME";
 
    case "$2" in
	EPM-31|EPMe-30|EPM-39|EPMe-42|EPM-43|EBX-38|EPMe-51|EPU-5120|EPU-5121)
	    FPGA_ADDR="0xC80";
	    SRC_LIB_NAME=$VL_LIB_NAME;
        LIB_CG="no";
	    ;;
	EPU-3311|EPU-3312)
	    FPGA_ADDR="0x1C80";
	    SRC_LIB_NAME=$CG_LIB_NAME;
        LIB_CG="yes";
	    ;;
	EPU-4011|EPU-4012)
	    FPGA_ADDR="0x1C80";
	    SRC_LIB_NAME=$VL_LIB_NAME;
        LIB_CG="no";
	    ;;
	EPU-4x62|EPU-4460)
	    FPGA_ADDR="0xC80";
	    SRC_LIB_NAME=$CG_LIB_NAME;
        LIB_CG="yes";
	    ;;
	EPM-19|EBX-18|MPEe-A1/A2|MPEe-U2)
	    echo "FPGA_BASE does not need to be set for this board.";
	    FPGA_ADDR="0xCA0";
	    SRC_LIB_NAME=$VL_LIB_NAME;
        LIB_CG="no";
	    ;;
	ESU-5070)
	    FPGA_ADDR="0xD80";
	    SRC_LIB_NAME=$MCU_LIB_NAME;
        LIB_CG="no";
	    ;;
	* )
	    echo "Unrecognized board name!";
	    FPGA_ADDR="0x000";
        printUsage;
        exit -1;
	    ;;
    esac

    echo "Setting FPGA_BASE=$FPGA_ADDR";
else
    printUsage;

    exit -1;
fi

KERNEL_HEADERS="/lib/modules/$KVER/build";
DRIVER_DIR="/lib/modules/$KVER";

# ***** Check for existence of kernel source directories. *****
if [ ! -d "$KERNEL_HEADERS" ]
then
    echo "Kernel header file source does not exist at: '$KERNEL_HEADERS'.";
    echo "Driver Installation Incomplete.";
    echo "Please install kernel headers and ensure they are symlinked at '$KERNEL_HEADERS'.";
    exit -1;
fi
echo "Found kernel source at:"$KERNEL_HEADERS"";


kDir=$KVER
ubuntuKSrc="/usr/src/linux-headers-$kDir";
centOSKSrc="/usr/src/kernels/$kDir";
userKSrc="";
for currentKSrc in "$userKSrc" "$ubuntuKSrc" "$centOSKSrc"
do
	echo "CHECKing for:$currentKSrc;";
done;

curDir=`pwd`;

KBUILD="/lib/modules/${KVER}/build"


for currentDriver in "vldrive" "vldriveax" "vldrivep" 
do
    echo "***** Compiling driver: "$currentDriver"";
    driverDir="${SCRIPT_DIR}/src/"$currentDriver"";
    if [ ! -d "$driverDir" ]
    then
        echo "Driver source directory does not exist: "$driverDir".";
        echo "Not building driver in directory: "$driverDir".";
    else
        cd $driverDir;

    make -C "${KBUILD}" M="${PWD}" clean >/dev/null 2>&1 || true
    
	    # vldrive is just a bit different from the others.
		if [ "$currentDriver" = "vldrive" ]
		then
            if [[ "$FPGA_ADDR" == "0xC80" ]]
            then
              sed "s/VL_FPGA_ADDR/$FPGA_ADDR/g" vldrive.h.template > vldrive.h
            fi
            if [[ ${FPGA_ADDR} == "0x1C80" ]]
            then
              sed "s/VL_FPGA_ADDR/$FPGA_ADDR/g" vldrive.h.template > vldrive.h
            fi
            if [[ ${FPGA_ADDR} == "0xD80" ]]
            then
              sed "s/VL_FPGA_ADDR/$FPGA_ADDR/g" vldrive.h.template > vldrive.h
            fi
		fi
    
    make -C "${KBUILD}" M="${PWD}" modules

    cd "$curDir";
	fi
done;
