#ifndef MY_ZSH_H
#define MY_ZSH_H
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <ctype.h>

void fill_slash_null(char *g_line, size_t olch)
{
    for (size_t s = 0; s < olch; s++)
    {
        g_line[s] = '\0';
    }
}

void removeExtraSpaces(char *str)
{
    int i, l;
    int for_space = 1;

    for (i = 0, l = 0; str[i] != '\0'; i++){
        if (isspace((unsigned char)str[i]))
        {
            if (for_space == 0)
            {
                str[l++] = ' ';
                for_space = 1;
            }
        }
        else
        {
            str[l++] = str[i];
            for_space = 0;
        }
    }
    str[l] = '\0';
}
#endif