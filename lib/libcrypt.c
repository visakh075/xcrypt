#include "libcrypt.h"
#include <stdio.h>
size_t len(char *in){size_t m=0;while(1){if(in[m]!='\0')m++;else break;}return(m);}
void * crypt(void * args)
{
    crypt_args * args_in;
    args_in=(crypt_args *)args;
    printf("i/p: %s\no/p:%s",args_in->in,args_in->out);
}