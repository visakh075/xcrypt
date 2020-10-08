#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<sys/stat.h>
#define KEYLEN 32
char Key[KEYLEN+1];
char FILELIST[500][150];
int FILESIZES[500];
int CFILESIZES[150];

float STIME[150]={1};
int NUM_FILES=1;
int NUM_DIR=0;

clock_t TIME;
int C_BASE=0;
int C_OFFSET=0;
int C_NUM=0;
int COMPFLAG[150];
int FOLDS;
int SETS;
int REMDR;
char FROM[150][150],CFROM[150];
char TO[150][150],CTO[150];
int CONF_OFFSET;

#define C_RED "\x1b[31m"
#define C_GREEN "\x1b[32m"
#define C_YELLOW "\x1b[33m"
#define C_BLUE "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN "\x1b[36m"
#define C_RST "\x1b[0m"
static void _mkdir(const char *dir) {
        char tmp[256];
        char *p = NULL;
        size_t len;
 
        snprintf(tmp, sizeof(tmp),"%s",dir);
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        mkdir(tmp, S_IRWXU);
                        *p = '/';
                }
        mkdir(tmp, S_IRWXU);}

float Size_formated(float size){
	while(size>1024)
	{
		size/=1024;
	}
	return(size);}
char char_formated(float size){	
	int i=0;
	char L[]={' ','K','M','G'};
	while(size>1024)
	{
		size/=(float)1024;
		i++;
	}
	return(L[i]);}

void clear_screen(){
	#ifdef WINDOWS
	    system("cls");
	#else
	    system ("clear");
	#endif
	}
int GEN_FILELIST(int cDIR){

	struct dirent *de;
	char FROM_DIR[150]="./";
	//printf("%s\n",FROM_DIR);fflush(stdout);	
	strcat(FROM_DIR,FROM[cDIR]);
	//printf("%s\n",FROM_DIR);fflush(stdout);	
	DIR *dr = opendir(FROM_DIR);

	if (dr == NULL) 
	{ 
		printf("SOUCE DIRECTORY DOESNOT EXIST" );fflush(stdout);
		return(0);
	} 
	int i=0;
	char filename[150];
	
	while (((de = readdir(dr)) != NULL)) 
			{
				strcpy(filename,de->d_name);
				if(strcmp(filename,".") && strcmp(filename,"..") )
				{
				strcpy(FILELIST[i++],filename);
				//printf("%s\n",FILELIST[i-1]);fflush(stdout);
				}
			}

	closedir(dr);
	
	for(int x =0;x<i;x++)
	{
		FILE *OriginalFile;
		char ofilename[150]={'\0'};
		
		strcat(ofilename,FROM[cDIR]);
		strcat(ofilename,"/");
		strcat(ofilename,FILELIST[x]);
			
		OriginalFile=fopen(ofilename,"rb");

		fseek(OriginalFile,0,SEEK_END);
		int sz=ftell(OriginalFile);
		FILESIZES[x]=sz;
		fclose(OriginalFile);
	}

	return i; }	
void GET_KEY(){
	
	FILE *KEYFILE,*CONFIG,*CONFIG_SET;
	
	KEYFILE=fopen("KEY","rb");
	if(KEYFILE==NULL)
	{printf("KEY NOT FOUND.\n");}
	fread(&Key,sizeof(char),KEYLEN,KEYFILE);
	fclose(KEYFILE);
	
	int CONFIG_SET_LEN;
	CONFIG_SET=fopen("OFFSET","rb");
	if(CONFIG_SET==NULL)
	{
		{printf("OFFSET SET TO DEFAULT.\n");}
		C_OFFSET=10;
		CONF_OFFSET=10;
	}
	else
	{
		fscanf(CONFIG_SET,"%d",&CONFIG_SET_LEN);
		if(CONFIG_SET_LEN!=0)
		{C_OFFSET=CONFIG_SET_LEN;CONF_OFFSET=C_OFFSET;}
		else
		{
			C_OFFSET=10;
			CONF_OFFSET=10;
		}
	}
	
	
	CONFIG=fopen("DIR_LIST","rb");
	if(CONFIG==NULL)
	{printf("DIR_LIST NOT FOUND.\n");}
	
	int COUNT=0;
	char xFROM[150];
	char xTO[150];
	int SKIPFLAG;
	while(!feof(CONFIG))
	{
		fscanf(CONFIG,"%d %s %s\n",&SKIPFLAG,xFROM,xTO);
		//mkdir(TO[COUNT],0777);
		if(SKIPFLAG==1)
		{
			FROM[COUNT][0]='\0';
			TO[COUNT][0]='\0';
			strcat(FROM[COUNT],xFROM);
			strcat(TO[COUNT],xTO);
			_mkdir(TO[COUNT]);
			COUNT++;
		}
		//_mkdir(TO[COUNT]);
		//COUNT++;
	}
	fclose(CONFIG);
	NUM_DIR=COUNT;}
void *ENCRYPT(void * FILENAME){
	
	FILE *OriginalFile,*EncryptedFile;
	
	char ofilename[150]={'\0'};
	strcat(ofilename,CFROM);
	strcat(ofilename,"/");
	strcat(ofilename,FILENAME);
	
	char EncryptedFileName[150]={'\0'};
	strcat(EncryptedFileName,CTO);
	strcat(EncryptedFileName,"/");	
	strcat(EncryptedFileName,FILENAME);
	
	//printf("\n%s (%ld)",EncryptedFileName,strlen(EncryptedFileName));
	//fflush(stdout);
	
	OriginalFile=fopen(ofilename,"rb");
	if(OriginalFile==NULL)
		{
			printf("FILE MISSING ENCRY %s\n",ofilename);fflush(stdout);return(NULL);
		}
	else
		{
			EncryptedFile=fopen(EncryptedFileName,"w");

			//fseek(OriginalFile,0,SEEK_END);
			//int sz=ftell(OriginalFile);
			//fseek(OriginalFile,0,SEEK_SET);
			int i=0;

			//for(int m=0;m<sz;m++)
			//
			do
			{
				char Sym,SymE;
				//fread(&Sym,sizeof(char),1,OriginalFile);
				Sym=fgetc(OriginalFile);
				SymE=Sym^Key[i++%KEYLEN];
				fputc(SymE,EncryptedFile);
				//fwrite(&SymE,sizeof(char),1,EncryptedFile);		
			}
			while(!feof(OriginalFile));
	}
	fclose(OriginalFile);
	fclose(EncryptedFile);

	return(NULL);}
void *STAT(){
	while(1)
	{
		usleep(250000);
		clear_screen();

		printf(C_RED"\n X"C_CYAN"\'"C_YELLOW"CRY"C_CYAN"PT\n");
		printf(C_BLUE" by HERMIT\n\n");
		printf(" KEY [");
		for(int m=0;m<KEYLEN;m++)
		{
			if(m%4==0){printf("-");}
			printf("%c",Key[m]);
			
		}
		printf("-]\n");
		
		printf(C_CYAN" DIR [%d/%d] %s -> %s\n",C_NUM,NUM_DIR,CFROM,CTO);
		printf(" SET [%d/%d] BASE [%d] OFFSET [%d]\n\n",C_BASE+1,SETS,C_BASE*CONF_OFFSET,C_OFFSET);
		int flag=1;
		
		int DSIZE;
		float TIME;
		float RATE;
		int ETA;
		int C_COMP=0;
		int MAX_ETA=0;
		for(int i=C_BASE*CONF_OFFSET;i<(C_OFFSET+(C_BASE*CONF_OFFSET));i++)
		{		if(FILESIZES[i]>0){
				FILE *EncryptedFile;
				char EncryptedFileName[150]={'\0'};
				strcat(EncryptedFileName,CTO);
				strcat(EncryptedFileName,"/");	
				strcat(EncryptedFileName,FILELIST[i]);
					
				EncryptedFile=fopen(EncryptedFileName,"rb");
				if(EncryptedFile==NULL)
				{
					printf(" NO FILE");fflush(stdout);
				}
				else
				{
					
					fseek(EncryptedFile,0,SEEK_END);
					int sz=ftell(EncryptedFile);
					fclose(EncryptedFile);

					float perc=(float)sz/FILESIZES[i];
					int precint=(int)(100*perc);
					
					TIME=((float)clock()-STIME[i])/CLOCKS_PER_SEC;
					STIME[i]=(float)clock();
					

					DSIZE=sz-CFILESIZES[i];
					CFILESIZES[i]=sz;

					//T_SIZE+=FILESIZES[i];
					//C_SIZE+=CFILESIZES[i];

					RATE=(float)(DSIZE/(TIME));
					ETA=0;
					if((int)RATE!=0)
					{ETA=(FILESIZES[i]-sz)/RATE;}
					if(ETA>MAX_ETA)
					{
						MAX_ETA=ETA;
					}
					COMPFLAG[i]=precint;

					flag*=(COMPFLAG[i]/100);
					printf(C_YELLOW" %-40.40s [",FILELIST[i]);
	
					
					if(COMPFLAG[i]==100)
					{printf(C_RST C_GREEN);C_COMP++;}
					else
					{printf(C_RST C_BLUE);}
					
					printf("%3.0d%%" C_RST,COMPFLAG[i]);
					printf(C_YELLOW"]" C_RST);}

					if(COMPFLAG[i]!=100)
					{
						printf(C_RED "\n [%.2f%cB] [%.2f%cB] [%-2.2d\'%-2.2d\"] [%-.2f%cB/s]" C_RST ,Size_formated((float)CFILESIZES[i]),char_formated(CFILESIZES[i]),Size_formated((float)FILESIZES[i]),char_formated(FILESIZES[i]),ETA/60,ETA%60,Size_formated(RATE),char_formated(RATE));
					}
					
				else{printf("\n");}printf("\n");
				}
			
		}
		//ELASP_TIME=clock()-ELASP_TIME;
		//float m=(float)ELASP_TIME/CLOCKS_PER_SEC;

		printf(C_CYAN"\n CPL [%d/%d] PRG [%d]\n ETA [%-2.2d\'%-2.2d\"]\n"C_RST,C_BASE*CONF_OFFSET+C_COMP,NUM_FILES,C_OFFSET-C_COMP,MAX_ETA/60,MAX_ETA%60);
		fflush(stdout);
		if(flag==1)
		{break;}
		
	}
	return(NULL);}

void ENCRYPT_DIR(int cDIR,int BASE,int OFFSET){
	FILE * RECORDS;
	RECORDS=fopen("RECORDS.txt","a");

	//NUM_FILES=GEN_FILELIST(cDIR);
	pthread_t ENCRYPTER[OFFSET];
	pthread_t STATID;
	for(int i=BASE;i<(OFFSET+BASE);i++)
	{
		if(FILESIZES[i]>0)
		{pthread_create(&ENCRYPTER[i-BASE],NULL,ENCRYPT,FILELIST[i]);}		

	}
	pthread_create(&STATID,NULL,STAT,NULL);
	for(int i=BASE;i<(OFFSET+BASE);i++)
	{
		if(FILESIZES[i]>0)
		{pthread_join(ENCRYPTER[i-BASE],NULL);}	
		
		fputs(FILELIST[i],RECORDS);
		fputc('\n',RECORDS);
	}
	pthread_join(STATID,NULL);
	fclose(RECORDS);}

int main(){

	GET_KEY();
	TIME=clock();
	for(int i=0;i<NUM_DIR;i++)
	{

		NUM_FILES=GEN_FILELIST(i);
		if(NUM_FILES==0){continue;}
		C_NUM=i+1;
		strcpy(CTO,TO[i]);
		strcpy(CFROM,FROM[i]);
		
		FOLDS=NUM_FILES/CONF_OFFSET;
		REMDR=NUM_FILES%CONF_OFFSET;
		
		if(REMDR!=0)
		{SETS=FOLDS+1;}
		else
		{SETS=FOLDS;}
		
		if(FOLDS>0)
		{	
			for(C_BASE=0;C_BASE<(FOLDS);C_BASE++)
			{
				C_OFFSET=CONF_OFFSET;
				ENCRYPT_DIR(i,(C_BASE*C_OFFSET),C_OFFSET);
			}
		}
		
		if(REMDR!=0||FOLDS<1)
		{
				C_OFFSET=REMDR;
				C_BASE=FOLDS;
				ENCRYPT_DIR(i,(C_BASE*CONF_OFFSET),C_OFFSET);
		} 
	}

	TIME=clock()-TIME;
	float TIMEINSEC=(float)TIME/CLOCKS_PER_SEC;
	printf(C_GREEN" R/T [%-2.2d\'%-2.2d\"]\n\n"C_RST,(int)TIMEINSEC/60,(int)TIMEINSEC%60);
}
