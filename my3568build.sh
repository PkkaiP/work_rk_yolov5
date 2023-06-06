#! /bin/bash

rm -r build
mkdir -p build
cd build

export PATH=/home/rockchip/SDK/rk3566-rk3568-linux/buildroot/output/rockchip_rk3568/host/bin:$PATH

qmake .. && make
