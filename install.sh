printf "\n \n"

echo -n " Installing Poco "

printf "\n ---------------------- \n"

# sudo apt-get -y install libpoco-dev

printf "\n \n"

echo -n " Installing Boost "

printf "\n ---------------------- \n"

# sudo apt-get -y install libboost1.71-all-dev

printf "\n \n"

echo -n " Installing SSH "

printf "\n ---------------------- \n"

sudo apt-get -y install libssh-dev

printf "\n \n"

echo -n " Installing nlohmann-json3 "

printf "\n ---------------------- \n"

sudo apt-get install nlohmann-json3-dev

printf "\n \n"

echo -n " Installing cpp-jwt "

printf "\n ---------------------- \n"

git clone https://github.com/arun11299/cpp-jwt.git
cd cpp-jwt
mkdir build
cd build
cmake .. -DCPP_JWT_BUILD_EXAMPLES=OFF -DCPP_JWT_BUILD_TESTS=OFF
cmake --build . --target all -j8
sudo make install -j8
cd ../..
rm -rf cpp-jwt
cd