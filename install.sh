#!/usr/bin/env bash

set -e
mkdir -p build
pushd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/ -DCDS_DEBUG=false
make -j ${nproc}
sudo make install
