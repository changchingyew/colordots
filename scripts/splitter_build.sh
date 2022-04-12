#!/bin/bash
set -x
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icpx
make
