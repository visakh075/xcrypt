#ifndef _XCRYPT_
#define _XCRYPT_
#include<stdio.h>
typedef struct crypt_args{
    char * in;
    char * out;
}crypt_args;
void * crypt(void * crypt_args);
size_t len(char * in);
#endif