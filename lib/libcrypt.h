#ifndef _XCRYPT_
#define _XCRYPT_
#include<stdio.h>
#include<pthread.h>
extern char Key[16];
extern size_t FILESIZE;
extern size_t BUFF;
extern int stop;
extern char * FILENAME;
typedef struct crypt_args{
    char * in;
    char * out;
}crypt_args;
void * thread_crypt(void * crypt_args);
void * stat();
size_t filelen(char * file);
size_t len(char * in);
void xcrypt(char * in,char * out);
#endif