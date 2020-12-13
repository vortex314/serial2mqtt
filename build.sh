echo "run from workspace directory !"
#  INSTALL OPENSSL dev libraries first
sudo apt-get install libssl-dev
#
git clone https://github.com/eclipse/paho.mqtt.c.git
git clone https://github.com/bblanchon/ArduinoJson
git clone https://github.com/vortex314/Common
git clone https://github.com/vortex314/serial2mqtt
cd Common
make -f Common.mk
cd ../paho.mqtt.c
make
cp ../serial2mqtt/makePaho.sh .
./makePaho.sh 
cd ../serial2mqtt
#
# change all paths in the .mk file to your paths
#
make -f serial2mqtt.mk
# for the build of the binary copy
sudo apt-get install zip
make -f serial2mqtt.mk PostBuild
 
