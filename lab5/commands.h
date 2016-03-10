#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>

int mymkdir (char *dirname);
int myrmdir(char *dirname);
int myrm(char *filename);
int mycat(char *filename);
int mycp(char *file1, char *file2);
int myls(char *dirname);
int ls_dir(char *dirname);
int ls_file(char *fname);
void mypwd();

#endif
