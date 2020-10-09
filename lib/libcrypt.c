#include "libcrypt.h"
#include <stdio.h>
size_t len(char *in){size_t m=0;while(1){if(in[m]!='\0')m++;else break;}return(m);}
void * crypt(void * data)
{
    crypt_args * args=(crypt_args *)data;
    char * in=args->in;
    char * out=args->out;
    FILE * in_stream, * out_stream;
    in_stream=fopen(in,"rb");
    if(len(out)==0) out_stream=fopen(out,"r+b"); else out_stream=fopen(out,"wb");

    if(len(in)>0 && len(out)>0)
    {
        
        

        if(in_stream!=NULL)
        {
            printf("no in error");
        }
        if(out_stream!=NULL)
        {
            printf("no out error");
        }
    }
    //printf("i/p: %s %d\no/p: %s %d\n",args->in,len(args->in),args->out,len(args->out));
    return(NULL);
}