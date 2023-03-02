#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "${BASH_SOURCE[0]:-$0}")" >/dev/null 2>&1 ; pwd -P )"
. "${SCRIPT_DIR}/env.sh" 

# cd ${LOCAL_PATH}
# ./configure --prefix=${LOCAL_INSTALL} --enable-debug --enable-vaapi --enable-libmfx --enable-shared --disable-stripping
# ./configure --prefix=${LOCAL_INSTALL} --enable-debug --enable-vaapi --enable-libvpl --enable-shared --disable-stripping
# ./configure --prefix=${LOCAL_INSTALL} --enable-debug --enable-vaapi --enable-libmfx --enable-shared --disable-stripping --enable-libmtl --enable-protocol=mtl
# ./configure --prefix=${LOCAL_INSTALL} --enable-debug --enable-vaapi --enable-libmfx --enable-shared --disable-stripping --enable-libmtl
./configure --prefix=${LOCAL_INSTALL} --enable-debug --enable-vaapi --enable-libmfx --enable-shared --disable-stripping --enable-libmtl --enable-protocol=mtl --disable-optimizations
make -j$(nproc) V=1
# make install V=1
