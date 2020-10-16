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
    char * in;
    char * out;
    char * key;
    if(!error(map))
    {
        printf("no error");
        in=branch('.',map)->param[0];
        if(safe('n',map)){out=branch('n',map)->param[0];}
        //else{out=branch('.',map)->param[0];}
        
        if(safe('k',map)){GetKey(branch('k',map)->param[0]);key=branch('k',map)->param[0];}
        else{GetKey("KEY");}

        //in=branch('.',map)->param[0];
        encrypt(in,out);
    }
    else
    {
        printf("error\n");
    }
    //char * in="test/Revolonz.rar";
    //char * out="test/m.rar";
    
    //GenarateKey("test/KEY");
    //GetKey("test/KEY");
    //crypt((void *)&args);
    //encrypt(in,out);
    //encrypt(out,"test/out.zip");
    
    return(0);
}