#include "libcrypt.h"
#include <stdio.h>
#include<pthread.h>
extern char Key[16];
extern size_t FILESIZE;
extern size_t BUFF;
extern int stop;
extern char * FILENAME;
size_t len(char *in){size_t m=0;while(1){if(in[m]!='\0')m++;else break;}return(m);}
size_t filelen(char * filename){
    	FILE * OriginalFile=fopen(filename,"rb");
		fseek(OriginalFile,0,SEEK_END);
		size_t FILESIZE=ftell(OriginalFile);
		fclose(OriginalFile);
        return FILESIZE;
}
void * crypt(void * data)
{
    crypt_args * args=(crypt_args *)data;
    char * in=args->in;
    char * out=args->out;
    FILENAME=in;
    BUFF=0;
    stop=0;
    FILESIZE=filelen(in);

    FILE * in_stream, * out_stream;
    in_stream=fopen(in,"rb");
    if(len(out)==0) out_stream=fopen(out,"r+b"); else out_stream=fopen(out,"wb");
    if(in_stream!=NULL&&out_stream!=NULL)
    {
        char Sym,SymE;
        int i=0;
        //FILESIZE=filelen(in);
        while(!feof(in_stream))
        {
            Sym=fgetc(in_stream);
            SymE=Sym^Key[i%16];
            i++;
            fputc(SymE,out_stream);
            BUFF++;
            //fwrite("%c",sizeof(char),1,out_stream);
        }
        fclose(in_stream);fclose(out_stream);
        //BUFF=0;
        stop=1;
    }
    //printf("i/p: %s %d\no/p: %s %d\n",args->in,len(args->in),args->out,len(args->out));
    pthread_exit(NULL);
    return(NULL);
}
void * stat()
{
    printf("%s\n",FILENAME);
    while(!stop){
    printf("\r%3.0f ",(float)(BUFF*100)/FILESIZE);
    fflush(stdout);
    //printf("\r");
    }
    printf("\n");
    pthread_exit(NULL);
}
void encrypt(char * in,char * out)
{
    crypt_args args;
    args.in=in;
    args.out=out;
    pthread_t machine;
    pthread_t status;
    pthread_create(&machine,NULL,crypt,(void *)&args);
    pthread_create(&status,NULL,stat,NULL);
    pthread_join(machine,NULL);
    pthread_join(status,NULL);
}