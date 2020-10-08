cd lib
./make_all.sh
cd ..
gcc -Wall -o xcrypt xcrypt.c lib/libcrypt.o