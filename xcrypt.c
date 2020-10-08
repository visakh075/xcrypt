#include<stdio.h>
#include<stdlib.h>
#include "lib/libcrypt.h"
crypt_args args;
int main(int argc,char ** argv)
{
args.in="thermit";
args.out="podohey";
crypt((void *)&args);
}