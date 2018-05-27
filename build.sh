#  INSTALL OPENSSL dev libraries first
# sudo apt-get install libssl-dev
#
git clone https://github.com/eclipse/paho.mqtt.c.git
git clone https://github.com/pantor/inja.git
git clone https://github.com/nlohmann/json.git
git clone https://github.com/vortex314/Common
git clone https://github.com/vortex314/serial2mqtt
cd Common
make -f Common.mk
cd ../paho.mqtt.c
make
cp ../serial2mqtt/makePaho.sh .
./makePaho.sh 
cd ../serial2mqtt
make -f serial2mqtt.mk
