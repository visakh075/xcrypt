#include "keyman.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
extern char Key[16];
void GetKey(char * keyfile)
{
    //char Key[16];
    FILE * KeyFile;
    if(strlen(keyfile)>0)
    {
        KeyFile=fopen(keyfile,"rb");
        if(KeyFile==NULL)
        {
            printf("Key Error");
            exit(0);
        }
    }
    else
    {
        KeyFile=fopen("KEY","rb");
        if(KeyFile==NULL)
        {
            printf("Key Error");
            exit(0);
        }
    }
    
    fread(Key,1,16,KeyFile);
    fclose(KeyFile);
}
void GenarateKey(char * keyfile)
{
    char fKey[16];
    FILE * KeyFile;
    srand(time(0));
    for(int c=0;c<16;c++)
    {
        fKey[c]=(rand()%128);
    }
    if(strlen(keyfile)>0)
    {
        KeyFile=fopen(keyfile,"wb");
    }
    else
    {
        KeyFile=fopen("KEY","wb");
    }
    
    fwrite(fKey,1,16,KeyFile);
    fclose(KeyFile);
}