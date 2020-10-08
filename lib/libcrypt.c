#include "libcrypt.h"
#include <stdio.h>
size_t len(char *in){size_t m=0;while(1){if(in[m]!='\0')m++;else break;}return(m);}
void * crypt(void * data)
{
    crypt_args * args;
    args=(crypt_args *)data;
    printf("i/p: %s\no/p:%s\n",args->in,args->out);
    return(NULL);
}