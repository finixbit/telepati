#!/bin/sh

APP_BIN_DIR="$(pwd)"
APP_SRC_DIR="$(pwd)/src"


echo "[*]compiling config ..."
gcc -o $APP_SRC_DIR/config.o -c $APP_SRC_DIR/config.cc -O3 -std=gnu++11 -ljansson

echo "[*]compiling redis ..."
gcc -o $APP_SRC_DIR/redis.o -c $APP_SRC_DIR/redis.cc -O3 -std=gnu++11 -lhiredis

echo "[*]compiling grpc.transmitter ..."
g++ -std=c++11 -I/usr/local/include -pthread -c -o $APP_SRC_DIR/transmitter.pb.o \
	$APP_SRC_DIR/transmitter.pb.cc
g++ -std=c++11 -I/usr/local/include -pthread -c -o $APP_SRC_DIR/transmitter.grpc.pb.o \
	$APP_SRC_DIR/transmitter.grpc.pb.cc

echo "[*]compiling sender ..."
g++ -o $APP_SRC_DIR/sender.o -c $APP_SRC_DIR/sender.cc \
	-O3 -std=gnu++11 -lpthread -ltins \
	`pkg-config --libs grpc++ grpc` -lgrpc++_reflection -lprotobuf -lpthread 

echo "[*]compiling receiver ..."
g++ -o $APP_SRC_DIR/receiver.o -c $APP_SRC_DIR/receiver.cc \
	-O3 -std=gnu++11 -lpthread -ltins \
	`pkg-config --libs grpc++ grpc` -lgrpc++_reflection -lprotobuf -lpthread 

echo "[*]compiling core ..."
g++ -o $APP_SRC_DIR/core.o -c $APP_SRC_DIR/core.cc \
	-O3 -std=gnu++11 -lpthread -lhiredis \
	`pkg-config --libs grpc++ grpc` -lgrpc++_reflection -lprotobuf -lpthread 

echo "[*]compiling main ..."
g++ -o $APP_SRC_DIR/server $APP_SRC_DIR/main.cc \
	$APP_SRC_DIR/config.o $APP_SRC_DIR/redis.o \
	$APP_SRC_DIR/transmitter.grpc.pb.o $APP_SRC_DIR/transmitter.pb.o \
	$APP_SRC_DIR/sender.o $APP_SRC_DIR/receiver.o $APP_SRC_DIR/core.o \
	-O3 -std=gnu++11 -lpthread -ljansson -ltins -lhiredis \
	`pkg-config --libs grpc++ grpc` -lgrpc++_reflection -lprotobuf -lpthread 

