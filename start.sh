printf "\n \n"

echo -n " Update Project "

printf "\n ---------------------- \n"

git pull

printf "\n \n"

echo -n " Starting Building Project "

printf "\n ---------------------- \n"

echo -n " Create Build Directory "

printf "\n \n"

cd Web-Server

mkdir build

echo -n " Start Cmake "

printf "\n \n"

sudo chmod -R 755 build

cd build && sudo cmake ..

printf "\n ---------------------- \n"

echo -n " Start Cmake Build "

printf "\n  \n"

sudo cmake --build . --target all -j8

printf "\n ---------------------- \n"

echo -n " Building Complete "

printf "\n ---------------------- \n \n"

echo -n " Starting"

printf "\n \n \n"

cd Executable/ && ./Executable
