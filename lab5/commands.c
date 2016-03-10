//Erik Lystad and Megan McPherson
//for use with client.c and server.c

#include "commands.h"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int mymkdir(char *dirname)
{
  if (mkdir("newdir", 0777) < 0){
    printf("errno=%d : %s\n", errno, strerror(errno));
  return 0;
}

int myrmdir(char *dirname)
{
  rmdir(dirname);
  return 0;
}

int myrm(char *filename)
{
  unlink(filename);
  return 0;
}

//from KC's class notes #9
int mycat(char *filename)
{
  int fd;
  int i, n;
  char buf[1024];
  //if (argc < 2) exit(1);
  fd = open(filename, O_RDONLY);
  if (fd < 0) exit(2);
  while (n = read(fd, buf, 1024))
  {   
     for (i=0; i < n; i++){putchar(buf[i]); }
  }
  return 0;
}

//from KC's class notes #9
int mycp(char *file1, char *file2)
{
  #define BLKSIZE 4096
  int fd, gd;
  char buf[4096];                                   
  int n, total=0;                   
  //if (argc < 3) {return(1);}
  fd = open(file1, O_RDONLY);
  if (fd < 0) {return(2);}          
  gd=open(file2,O_WRONLY|O_CREAT);
  if (gd < 0) {return(3);}
  while (n=read(fd, buf, BLKSIZE))  
  {                                 
     write(gd, buf, n);       
     total += n;
  }                                 
  printf("total=%d\n",total);       
  close(fd); close(gd);
  return 0;                                        
}

//from KC's class notes #8
 int myls(char *dirname)
{
  struct stat mystat, *sp;
  int r;
  sp = &mystat;
  if (r = lstat(dirname, sp) < 0){
     printf("no such file %s\n", dirname); return(1);
  }
  if (S_ISDIR(sp->st_mode)) {ls_dir(dirname);}
  else {ls_file(dirname);}
  return 0;
}

int ls_dir(char *dirname)
{
  DIR *dp = opendir(dirname);
  struct dirent *d = readdir(dp);
  while(d != NULL)
  {
    printf("%s\n", d->d_name);
    d = readdir(dp);
  }
  return 0;
}

int ls_file(char *fname)
{
  struct stat fstat, *sp;
  int r, i;
  char ftime[64], linkname[64];

  sp = &fstat;
  //printf("name=%s\n", fname); getchar();

  if ( (r = lstat(fname, &fstat)) < 0){
     printf("can't stat %s\n", fname); 
     exit(1);
  }

  if ((sp->st_mode & 0xF000) == 0x8000)
     printf("%c",'-');
  if ((sp->st_mode & 0xF000) == 0x4000)
     printf("%c",'d');
  if ((sp->st_mode & 0xF000) == 0xA000)
     printf("%c",'l');

  for (i=8; i >= 0; i--){
    if (sp->st_mode & (1 << i))
	printf("%c", t1[i]);
    else
	printf("%c", t2[i]);
  }

  printf("%4d ",sp->st_nlink);
  printf("%4d ",sp->st_gid);
  printf("%4d ",sp->st_uid);
  printf("%8d ",sp->st_size);

  // print time
  strcpy(ftime, ctime(&sp->st_ctime));
  ftime[strlen(ftime)-1] = 0;
  printf("%s  ",ftime);

  // print name
  printf("%s", basename(fname));  

  // print -> linkname if it's a symbolic file
  if ((sp->st_mode & 0xF000)== 0xA000){ // YOU FINISH THIS PART
     // use readlink() SYSCALL to read the linkname
     readlink(fname, linkname, 64);
     printf(" -> %s", linkname);
  }
  printf("\n");
  return 0;
}

void mypwd()
{
  char buf[256];
  getcwd(buf, 256);
}
}



