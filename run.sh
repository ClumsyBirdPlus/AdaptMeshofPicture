#!/bin/bash

export LD_LIBRARY_PATH=$HOME/.local/lib
export AFEPACK_PATH=$HOME/.local/include/AFEPack
export AFEPACK_TEMPLATE_PATH=$AFEPACK_PATH/template/triangle

./main
