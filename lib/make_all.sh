gcc -o libcrypt.o -c libcrypt.c -lpthread
gcc -o argman.o -c argman.c
gcc -o keyman.o -c keyman.c
gcc -Wall -o hexcomp hexcomp.c