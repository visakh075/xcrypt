#ifndef ARGMAN
#define ARGMAN
typedef struct {
    char opt;
    int pos;
    int n_param;
    char ** param;}arg_group;
typedef struct{
    //search parameters
    char * opt;
    int * ex_param;
    int d_param;
    int * err;
    int * pos;
    int * conf;
    int * is_def;
    //whole
    char ** arg_list;
    int argc;
    int * ass;
    int n_branch;
    int * inv_pos;
    int * flag;
    //map param
    int ic;
    arg_group * branch;}arg_map;
void map_init(arg_map * map,char * list,int * ex_param,int * conf,int * is_def,int d_param,int argc,char *argv[]);
void showmap(arg_map map);
size_t argsize(char * filename);
int isopt(char *arg,char c);
void explore_map(arg_map * map);
void wars(arg_map map);
int safe(char c,arg_map map);
int error(arg_map map);
arg_group * branch(char c,arg_map map);
#endif