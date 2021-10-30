printf "\n \n"

printf "\n ---------------------- \n"
printf "         Update"
printf "\n ---------------------- \n\n"

apt-get update

printf "\n ---------------------- \n"
printf "    Installing Git, Cmake, GCC, build-essential"
printf "\n ---------------------- \n\n"

apt-get install -y git cmake libgtest-dev build-essential gcc-10 g++-10 

printf "\n ---------------------- \n"
printf "    Installing Poco"
printf "\n ---------------------- \n\n"

apt-get -y install libpoco-dev

printf "\n ---------------------- \n"
printf "    Installing Boost"
printf "\n ---------------------- \n\n"

apt-get -y install libboost-all-dev

printf "\n ---------------------- \n"
printf "     Installing SSH"
printf "\n ---------------------- \n\n"

apt-get -y install libssh-dev

printf "\n ------------------------- \n"
printf " Installing nlohmann-json3"
printf "\n ------------------------- \n\n"

apt-get install nlohmann-json3-dev

printf "\n ---------------------- \n"
printf "   Installing cpp-jwt"
printf "\n ---------------------- \n\n"

git clone https://github.com/arun11299/cpp-jwt.git
cd cpp-jwt
mkdir build
cd build
cmake .. -DCPP_JWT_BUILD_EXAMPLES=OFF -DCPP_JWT_BUILD_TESTS=OFF
cmake --build . --target all -j8
make install -j8
cd ../..
rm -rf cpp-jwt
cd

printf "\n ---------------------- \n"
printf "   Installation completed"
printf "\n ---------------------- \n\n"