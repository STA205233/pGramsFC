sudo apt update
sudo apt upgrade -y
sudo apt install -y cmake ninja-build g++ clang libboost-all-dev ruby-all-dev swig git libmysqlcppconn-dev mosquitto mosquitto-clients build-essential libreadline-dev pkg-config libmosquitto-dev  libmosquittopp-dev
mkdir -p .dep/install
mkdir -p .dep/build
mkdir -p .dep/source
cd .dep/source
git clone https://github.com/OdakaHirokazu/ANLNext --depth 1
git clone https://github.com/NevisNeutrinos/pGRAMSBalloon --depth 1
cd ../build
cmake ../source/ANLNext -G Ninja -DCMAKE_INSTALL_PREFIX=../install -DANLNEXT_USE_READLINE=ON 
cmake --build .
cmake --install .
export ANLNEXT_INSTALL="`pwd`/../install"