#include<stdio.h>
#include<stdlib.h>
#include "argman.h"
size_t argsize(char * filename);
void map_init(arg_map * map,char * list,int * ex_param,int * conf,int * is_def,int d_param,int argc,char *argv[]){
    size_t size=argsize(list);
    //search parameters
    map->opt=(char *)malloc(sizeof(char *)*size);
    map->pos=(int *)malloc(sizeof(int *)*size);
    map->ex_param=(int *)malloc(sizeof(int*) *size);
    map->conf=(int *)malloc(sizeof(int*)*size);
    map->is_def=(int *)malloc(sizeof(int *)*size);
    map->d_param=d_param;
    
    //whole
    map->arg_list=argv;
    map->argc=argc;
    map->flag=(int *)malloc(sizeof(int*)*argc);
    map->inv_pos=(int *)malloc(sizeof(int*)*argc);
    map->err=(int *)malloc(sizeof(int*)*size);
    map->ass=(int *)malloc(sizeof(int*)*size);
    map->opt=list;
    map->ex_param=ex_param;
    map->conf=conf;
    map->is_def=is_def;
    
    for(int i=0;i<argsize(list);i++){map->pos[i]=-1;map->err[i]=0;}
    for(int i=0;i<argc;i++){map->flag[i]=-1;map->inv_pos[i]=-1;map->inv_pos[i]=0;map->ass[i]=-1;}
    map->ass[0]=0;
    map->ic=0;
    explore_map(map);
    }
void showmap(arg_map map){
    printf("\nmap of: ");
    for(int i=0;i<map.argc;i++){printf("%s(%d,%d) ",map.arg_list[i],map.flag[i],map.ass[i]);}//printf("\n");
    //matrix
    
    printf("\nOpti\t");for(int m=0;m<argsize(map.opt);m++){printf("%c\t",map.opt[m]);}printf("\n");
    printf("Exp_p\t");for(int m=0;m<argsize(map.opt);m++){printf("%d\t",map.ex_param[m]);}printf("\n");
    printf("Pos\t");for(int m=0;m<argsize(map.opt);m++){printf("%d\t",map.pos[m]);}printf("\n");
    printf("Confl\t");for(int m=0;m<argsize(map.opt);m++){printf("%d\t",map.conf[m]);}printf("\n");
    printf("Error\t");for(int m=0;m<argsize(map.opt);m++){printf("%d\t",map.err[m]);}printf("\n");
    printf("is_def\t");for(int m=0;m<argsize(map.opt);m++){printf("%d\t",map.is_def[m]);}printf("\n");
    printf("safe\t");for(int m=0;m<argsize(map.opt);m++){printf("%d\t",safe(map.opt[m],map));}printf("\n");
    }  
size_t argsize(char * filename){
    size_t size=0;
    while(1)
    {if(filename[size]!='\0'){size++;}else{break;}}
    return(size);}
int isopt(char *arg,char c){
    int isopt=0;if(arg[0]=='-'&&arg[1]==c){isopt=1;}return(isopt);
    }
void explore_map(arg_map * map){
    size_t size=argsize(map->opt);
    //position scan
        int branch_count=0;
        for(int c_arg=1;c_arg<map->argc;c_arg++){
        for(int s_arg=0;s_arg<size;s_arg++){
            if(isopt(map->arg_list[c_arg],map->opt[s_arg]))
            {
                map->flag[c_arg]=1;
                map->pos[s_arg]=c_arg;
                map->inv_pos[c_arg]=s_arg;
                map->ass[c_arg]=c_arg;
                if(map->ex_param[s_arg]>0)
                {branch_count++;}
            }}}

    map->n_branch=(branch_count+1); // add one for main branch
    map->branch=(arg_group *)malloc(sizeof(arg_group)*map->n_branch);
    //
    //branch defintion
    int bi=0;
    for(int i=0;i<map->argc;i++){
        if((map->flag[i])>0 && map->ex_param[map->inv_pos[i]]) // opt and parameters >0
        {
            map->branch[bi].opt=map->opt[map->inv_pos[i]];
            map->branch[bi].pos=i;
            map->branch[bi].n_param=map->ex_param[map->inv_pos[i]];//map->flag[i];
            map->branch[bi].param=malloc(sizeof(char *)*map->branch[bi].n_param);
            for(int p=0;p<map->branch[bi].n_param;p++)
            {
                if(map->flag[i+p+1]<0)
                {
                    map->ass[i+p+1]=i;
                    map->branch[bi].param[p]=map->arg_list[i+p+1];
                }
                else
                {
                    map->branch[bi].param[p]=NULL;
                    map->err[map->inv_pos[i]]=1;
                }
                
            }bi++;
        }
        else if((map->flag[i])>0)
        {
            
        }
    }

    //
    //main branch
        
        for(int i=0;i<map->argc;i++){if(map->ass[i]<0){map->ic++;}}

        map->branch[bi].opt='.';
        map->branch[bi].pos=0;
        map->branch[bi].param=(char **)malloc(sizeof(char *)*map->d_param);
        map->branch[bi].n_param=map->d_param;
        int m=0;
        for(int i=0;i<map->argc;i++)
            {
            if(map->ass[i]<0)
            {map->branch[bi].param[m]=map->arg_list[i];m++;}
            }
    //
    //conflict search
        for(int i=0;i<argsize(map->opt);i++){
            for(int j=0;j<argsize(map->opt);j++)
            {
                if(map->conf[i]==map->conf[j] && i!=j && map->pos[i]>-1 && map->pos[j]>-1)
                {
                    map->err[i]=2;
                }
            }
            }
}
void wars(arg_map map){
    printf("\n");
    for(int i=0;i<argsize(map.opt);i++)
    {
        int err=map.err[i];
        if(err==1)
        {
            printf("%c argumnent missing ,",map.opt[i]);
        }
        if(err==2)
        {
            printf(" %c conflict error ,",map.opt[i]);
        }
    }
    if(map.d_param!=map.ic){printf("i error ,");}
    printf("\n");
}
int safe(char c,arg_map map)
{
    int safe=0;
    // non conflict
    // 
    for(int m=0;m<argsize(map.opt);m++)
    {
        if(map.opt[m]==c)
        {
            int is_conf=0;// searching for other this opt is default
            for(int n=0;n<argsize(map.opt);n++)
            {
                if(map.conf[m]==map.conf[n] && map.conf[n]>-1)// same conf
                {
                    if(map.pos[n]>0) // same conf number and the other elemnt exist
                    {
                        is_conf=1;break;
                    }
                }

            }

            if((map.err[m]==0 && map.pos[m]>0) || (map.is_def[m]==1 && !is_conf))
            {
               safe=1;break;
            }
            //if(map.pos[m]>0 || map.is_def[m]>0)
            //{
            //    if(map.err[m]==0 && map.ic==map.d_param)
            //    {
            //        safe=1;break;
            //    }
            //}
        }
    }
    return(safe);
}
int error(arg_map map){
    for(int i=0;i<argsize(map.opt);i++)
    {if(map.err[i]>0) return(1);}
    if(map.d_param!=map.ic) return(1);
    return(0);
}
arg_group * branch(char c,arg_map map)
{
    for(int i=0;i<argsize(map.opt);i++)
    {
        if(map.branch[i].opt==c)
        {
            return(&map.branch[i]);
        }
    }
    return(NULL);
}