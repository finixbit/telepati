#!/bin/sh

export LD_LIBRARY_PATH=$(pwd)/libs
$(pwd)/src/server --config config.json --server $1
