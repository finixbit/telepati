#!/bin/sh

protoc -I $(pwd) --cpp_out=$(pwd)/src \
	$(pwd)/transmitter.proto

protoc -I $(pwd) --grpc_out=$(pwd)/src \
	--plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $(pwd)/transmitter.proto


# node js grpc code generation
# read link below for setup
# https://io2015codelabs.appspot.com/codelabs/gRPC#2
protoc --proto_path=$(pwd) \
	   --js_out=import_style=commonjs,binary:$(pwd)/webapp/services \
	   --grpc_out=$(pwd)/webapp/services \
	   --plugin=protoc-gen-grpc=/usr/local/bin/grpc_node_plugin \
	   $(pwd)/transmitter.proto