#include<stdio.h>
size_t getfs(char *filename);
int main(int argc, char *argv[])
{
	FILE *a,*b;
	a=fopen(argv[1],"rb");
	b=fopen(argv[2],"rb");
	size_t len_a=getfs(argv[1]);
	size_t len_b=getfs(argv[2]);

	if(len_a!=len_b)
	{
		printf("\nSize Miss-match\n\n");
	}
	
	int miss_count=0;
	unsigned char m,n;
	for(size_t pos=0;pos<len_a;pos++)
	{
		m=getc(a);
		n=getc(b);
		
		if(m!=n)
		{
			miss_count++;
		}
	}
	if(miss_count>0)
	{
		printf("\nByte Miss-match\nTotal Missmatch %d of %lu (%02lu)\n\n",miss_count,len_a,(miss_count+1)*100/len_a);
	}
	if((miss_count<1)&&(len_a==len_b))
	{
		printf("\nComplete Match\n\n");
	}

}
size_t getfs(char *filename){
	FILE *fp;
	fp=fopen(filename,"rb");
	fseek(fp, 0L, SEEK_END); 
	size_t res = ftell(fp);
    fclose(fp);
	return(res);
	}
