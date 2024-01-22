#include "include/my_zsh.h"
#include <stdio.h>
#include <stdlib.h>
#define CASE_MAX_BUFF 256
#define CASE_MAX_TOKENS 64
static char prev_dir[CASE_MAX_BUFF] = "";

char **function_split(char *input)
{
    removeExtraSpaces(input);
    char** sh_case = (char**)malloc(CASE_MAX_TOKENS * sizeof(char*));
    if (!sh_case) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    char* new_dub = strdup(input);
    if (!new_dub) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int shcase_count = 0;
    char* splited_words = strtok(new_dub, " ");
    while (splited_words != NULL && shcase_count < CASE_MAX_TOKENS - 1) {
        sh_case[shcase_count] = strdup(splited_words);
        if (!sh_case[shcase_count]) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        splited_words = strtok(NULL, " ");
        shcase_count++;
    }

    sh_case[shcase_count] = NULL;
    free(new_dub);
    return sh_case;
}

void command_func_env(char **env_z)
{
    int sh = 0;
    while (env_z[sh]){
        printf("%s\n", env_z[sh]);
        sh++;
    }
}

void command_func_pwd(size_t olch)
{
    char *create_arrayuse = (char *)calloc(CASE_MAX_BUFF, sizeof(char));
    if (getcwd(create_arrayuse, olch))
    {
        printf("%s\n", create_arrayuse);
    }
    free(create_arrayuse);
}

int checker_fpath(char* args, char** path){
    char* mal_strs = malloc(PATH_MAX);
    strcpy(mal_strs ,getenv("PATH"));
    char* box_fuse = strtok(mal_strs, ":");
    char* kmp;
    int dlina;
    while (box_fuse != 0) {
        dlina = (strlen(box_fuse) + strlen(args) + 2);
        kmp = (char*)calloc(dlina, sizeof(char));
        snprintf(kmp, dlina, "%s/%s", box_fuse, args);
        if(access(kmp, X_OK) == 0){
            *path = strdup(kmp);
            free(kmp);
            free(mal_strs);
            return 1;
        }
        box_fuse = strtok(NULL, ":");
        free(kmp);
    }
    free(mal_strs);
    free(box_fuse);
    return 0;
}


int executor(char **args, char** env)
{
    pid_t g_pid;
    int status_fp;
    char* g_path_exec = NULL;
    if(strstr(args[0], "./") != NULL){
        g_path_exec = realpath(args[0], g_path_exec);
        if(access(g_path_exec, X_OK) != 0){
            printf("error in access");
            return 0;
        }
        if(g_path_exec == NULL){
            printf("error in path");
        }
    }else if(checker_fpath(args[0], &g_path_exec) == 0){
        free(g_path_exec);
        printf("my_zsh: command not found - '%s'\n", args[0]);
        return 0;
    }

    g_pid = fork();
    if (g_pid == 0) {
        if (execve(g_path_exec, args, env) == -1) {
            perror("lsh");
        }
        free(g_path_exec);
        exit(EXIT_FAILURE);
    } else {
        do {
            waitpid(g_pid, &status_fp, WUNTRACED);
            if(WIFSIGNALED(status_fp == SIGSEGV)){
                printf("segmentation fault\n");
            }
        } while (!WIFEXITED(status_fp) && !WIFSIGNALED(status_fp));
    }
    free(g_path_exec);
    return 1;
}

void command_func_echo(char ** comm_ray)
{
    if ( comm_ray[1] != NULL)
    {
        if ( comm_ray[1][0] == '$'){
            char *f_box;
            if (!(f_box = getenv(&( comm_ray[1][1]))))
            {
                printf("env: not found\n");
                return;
            }
            printf("%s\n", f_box);
        }else{
            for (int i = 1;  comm_ray[i] != NULL; i++){
                printf("%s ",  comm_ray[i]);
            }
            printf("\n");
        }
    }else{
        printf("echo: missing argument\n");
    }
}


void command_func_setenv(char **get_comray)
{
    if (strchr(get_comray[1], '=') != NULL){
        int p = 1;
        char *splited_words = strtok(get_comray[1], "=");
        while (splited_words != NULL){
            get_comray[p++] = strdup(splited_words);
            splited_words = strtok(NULL, "=");
        }
    }
    else if (strchr(get_comray[2], '=') != NULL){
        if (get_comray[3] != NULL){
            free(get_comray[2]);
            get_comray[2] = strdup(get_comray[3]);
        }
    }
    if (setenv(get_comray[1], get_comray[2], 0) == -1){
        printf("error setenv\n");
    }
}

void command_func_cd(char **line_comray)
{
    char stay_currdir[CASE_MAX_BUFF];

    if (line_comray[1] != NULL)
    {
        if (strcmp(line_comray[1], "-") == 0)
        {
            if (prev_dir[0] != '\0')
            {
                strcpy(stay_currdir, prev_dir);
                fill_slash_null(prev_dir, CASE_MAX_BUFF);
                getcwd(prev_dir, CASE_MAX_BUFF);
                chdir(stay_currdir);
            }
            else
            {
                printf("cd: no previous directory available\n");
            }
        }else{
            getcwd(prev_dir, CASE_MAX_BUFF);
            if (chdir(line_comray[1]) != 0)
            {
                printf("cd: no such file or directory: '%s'\n", line_comray[1]);
            }
        }
    }
}


void commands_sorting_type(char *get_memory, size_t len_siz, char **env_z){
    char** commands_ray = function_split(get_memory);

    if ( commands_ray[0] != NULL){
        if (strcmp( commands_ray[0], "env") == 0)
        {
            command_func_env(env_z);
        }
        else if (strcmp( commands_ray[0], "pwd") == 0)
        {
            command_func_pwd(len_siz);
        }
        else if (strcmp( commands_ray[0], "echo") == 0)
        {
            command_func_echo( commands_ray);
        }
        else if (strcmp( commands_ray[0], "setenv") == 0)
        {
            command_func_setenv( commands_ray);
        }
        else if (strcmp( commands_ray[0], "unsetenv") == 0)
        {
            if (unsetenv( commands_ray[1]) == -1)
            {
                printf("error unsetenv\n");
            }
        }
        else if (strcmp( commands_ray[0], "cd") == 0)
        {
            command_func_cd( commands_ray);
        }
        else if (strcmp( commands_ray[0], "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }else{
            executor(commands_ray, env_z);
        }
    }
    for(int i=0; commands_ray[i] != NULL; i++)
    {
        free(commands_ray[i]);
    }
    // free(commands_ray);
    // fill_slash_null(get_memory, len_siz);
}


void prompt_func(char **env)
{
    char *open_memory = (char *)malloc(CASE_MAX_BUFF * sizeof(char));
    size_t len_mbuff = CASE_MAX_BUFF;

    while (1){
        fill_slash_null(open_memory, len_mbuff);
        getcwd(open_memory, len_mbuff);
        printf("[%s]>", open_memory);
        fill_slash_null(open_memory, len_mbuff);
        if (getline(&open_memory, &len_mbuff, stdin) == -1){
            break;
        }
        commands_sorting_type(open_memory, len_mbuff, env);
    }
    fill_slash_null(open_memory, len_mbuff);
    free(open_memory);
}


int main(int argc, char **argv, char **env_z){
    if (argc && argv){}
    prompt_func(env_z);
    return 0;
}