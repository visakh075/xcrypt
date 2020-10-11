#include<stdio.h>
#include<stdlib.h>
#include "lib/libcrypt.h"
#include "lib/keyman.h"
#include "lib/argman.h"
char Key[16];char * FILENAME;
size_t FILESIZE=0,BUFF=0;
int stop=0;
int main(int argc,char ** argv)
{
    arg_map map;
    char * list="nrk";
    int ex_param[]={1,0,1};
    int conf[]={0,0,-1};
    int isdef[]={0,1,0};
    map_init(&map,list,ex_param,conf,isdef,1,argc,argv);
    //showmap(map);
    char * in;//="test/Lab.zip";
    char * out;//="test/m.zip";
    if(!error(map))
    {
        if(safe('n',map)==1)
        {
            out=branch('n',map)->param[0];
            printf("new %s",out);
        }
        else if(safe('r',map)==1)
        {
            out=branch('r',map)->param[0];
            printf("replace %s",out);
        }
        if(safe('k',map))
        {
            printf("key %s",branch('k',map)->param[0]);
        }
        in=branch('.',map)->param[0];
    }
    else
    {
        printf("error\n");
    }
    
    //char * in="test/Lab.zip";
    //char * out="test/m.zip";
    
    //GenarateKey("test/KEY");
    //GetKey("test/KEY");
    //crypt((void *)&args);
    //encrypt(in,out);
    //encrypt(out,"test/out.zip");
    
    return(0);
}