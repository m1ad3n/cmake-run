#!/bin/bash

cmake -S ./ -B build
cd ./build
make
sudo cp ./crun ~/bin/crun
echo "Done."