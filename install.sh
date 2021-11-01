printf "\n \n"

printf "\n ---------------------- \n"
printf "         Update"
printf "\n ---------------------- \n\n"

apt-get update

printf "\n -------------------------- \n"
printf "    Installing base utils"
printf "\n -------------------------- \n\n"

apt-get install -y git wget cmake libgtest-dev build-essential gcc-10 g++-10 

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

printf "\n ----------------------------- \n"
printf "   Installing nlohmann-json3"
printf "\n ---------------------------- \n\n"

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
printf "   Installing MongoDB"
printf "\n ---------------------- \n\n"

wget -qO - https://www.mongodb.org/static/pgp/server-5.0.asc | apt-key add -
apt update
apt-get install gnupg
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/5.0 multiverse" | tee /etc/apt/sources.list.d/mongodb-org-5.0.list
apt-get update
apt-get install -y mongodb-org
apt-get install -y mongodb-org=5.0.2 mongodb-org-database=5.0.2 mongodb-org-server=5.0.2 mongodb-org-shell=5.0.2 mongodb-org-mongos=5.0.2 mongodb-org-tools=5.0.2
systemctl start mongod

printf "\n ---------------------- \n"
printf "    Installing Redis"
printf "\n ---------------------- \n\n"

apt-get install -y redis

printf "\n -------------------------- \n"
printf "   Installation completed"
printf "\n -------------------------- \n\n"