mkdir build
conan install . --output-folder=build --build=missing
cd build
cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=/home/lieven/workspace/serial2mqtt/build/build/Release/generators/conan_toolchain.cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release
make