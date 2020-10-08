#include<stdio.h>
#include<stdlib.h>
#include "lib/libcrypt.h"

int main(int argc,char ** argv)
{
crypt_args args;
args.in="thermit";
args.out="podohey";
crypt((void *)&args);
return(0);
}