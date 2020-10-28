cd lib
bash make_all.sh
cd ..
gcc -Wall -o xcrypt xcrypt.c lib/libcrypt.o lib/keyman.o lib/argman.o -lpthread
