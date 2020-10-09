#include<stdio.h>
#include<stdlib.h>
#include "lib/libcrypt.h"
#include "lib/keyman.h"
char Key[16];
size_t FILESIZE=0,BUFF=0;
int stop=0;
int main(int argc,char ** argv)
{
char * in="test/Lab.zip";
char * out="test/m.zip";
//GenarateKey("test/KEY");
GetKey("test/KEY");
//crypt((void *)&args);
encrypt(in,out);
encrypt(out,"test/out.zip");
return(0);
}