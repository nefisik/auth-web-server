#!/bin/bash

printf "\n ---------------------- \n"
printf " Create Build Directory"
printf "\n ---------------------- \n\n"

cd /usr/src/auth-web-server
mkdir build
cd build

printf "\n ---------------------- \n"
printf "      Start build"
printf "\n ---------------------- \n\n"

cmake ..
cd Executable
cmake --build . --target all -j8

printf "\n ---------------------- \n"
printf "   Building Complete"
printf "\n ---------------------- \n \n"

./Executable