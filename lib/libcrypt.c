#include "libcrypt.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
extern char Key[16];
extern size_t FILESIZE;
extern size_t BUFF;
extern int stop;
extern char * FILENAME;
size_t len(char *in){size_t m=0;if(in==NULL){return(0);} while(1){if(in[m]!='\0')m++;else break;}return(m);}
size_t filelen(char * filename){
    	FILE * OriginalFile=fopen(filename,"rb");
        if(OriginalFile==NULL){return(0);}
		fseek(OriginalFile,0,SEEK_END);
		size_t FILESIZE=ftell(OriginalFile);
		fclose(OriginalFile);
        return FILESIZE;
}
void * thread_crypt(void * data)
{
    crypt_args * args=(crypt_args *)data;
    char * in=args->in;
    char * out=args->out;
    FILENAME=in;
    BUFF=1;
    stop=0;
    
    FILESIZE=filelen(in);
    if(FILESIZE==0){printf("error : non-vaid file");}
    else{
    FILE * in_stream, * out_stream;
    in_stream=fopen(in,"rb");
    if(len(out)==0){out_stream=fopen(in,"r+b");} else{out_stream=fopen(out,"wb");}
    if(in_stream!=NULL&&out_stream!=NULL)
    {
        char Sym,SymE;
        int i=0;
        while(!feof(in_stream))
        {
            Sym=fgetc(in_stream);
            SymE=Sym^Key[i%16];
            i++;
            fputc(SymE,out_stream);
            BUFF++;
        }
        fclose(in_stream);fclose(out_stream);
        stop=1;
    }
    }
    pthread_exit(NULL);
    return(NULL);
}
void * stat()
{
    
    while(!stop){
        usleep(50000);
        printf("\r");
        printf("%-15.15s \t",FILENAME);
        printf("%3.0f ",(float)(BUFF*100)/FILESIZE);
        fflush(stdout);
    }
    printf("\n");
    pthread_exit(NULL);
}
void xcrypt(char * in,char * out)
{
    crypt_args args;
    args.in=in;
    args.out=out;
    pthread_t machine;
    pthread_t status;
    pthread_create(&machine,NULL,thread_crypt,(void *)&args);
    pthread_create(&status,NULL,stat,NULL);
    pthread_join(machine,NULL);
    pthread_join(status,NULL);
}