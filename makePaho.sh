${CC:-cc} -c -g -fPIC -Os -Wall -Ibuild src/Clients.c src/Heap.c  src/MQTTAsync.c \
src/MQTTAsyncUtils.c src/MQTTTime.c \
src/LinkedList.c src/Log.c src/Messages.c src/MQTTClient.c \
src/MQTTPacket.c src/MQTTPacketOut.c src/MQTTPersistence.c \
src/MQTTPersistenceDefault.c src/MQTTProtocolClient.c \
src/MQTTProtocolOut.c src/SocketBuffer.c src/Socket.c src/StackTrace.c \
src/Thread.c src/Tree.c src/utf-8.c src/MQTTProperties.c src/WebSocket.c src/SHA1.c src/Base64.c 
${AR:-ar} -cru build/output/libpaho-mqtt3c.a Clients.o  Heap.o MQTTAsync.o \
MQTTAsyncUtils.o MQTTTime.o \
LinkedList.o  Log.o  Messages.o  MQTTClient.o  MQTTPacket.o \
MQTTPacketOut.o  MQTTPersistence.o  MQTTPersistenceDefault.o \
MQTTProtocolClient.o  MQTTProtocolOut.o  SocketBuffer.o  Socket.o \
StackTrace.o  Thread.o  Tree.o MQTTProperties.o WebSocket.o SHA1.o Base64.o utf-8.o
# ${CC:-cc} -static -o build/output/samples/paho_cs_sub src/samples/paho_cs_sub.c build/output/libpaho-mqtt3c.a -I src -lpthread -L build/output
