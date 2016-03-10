// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include "commands.h" //LAB 5 CODE

#include <fcntl.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>

#define MAX 256

//run after running server.c
//run with arguments IP and Port from server.c

// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 

int server_sock, r;
int SERVER_IP, SERVER_PORT; 
char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int mymkdir (char *dname);
int myrmdir(char *dname);
int myrm(char *filename);
int mycat(char *filename);
int mycp(char *file1, char *file2);
int myls(char dname[128]);
int ls_dir(char *dname);
int ls_file(char *fname);
void mypwd();
void mycd(char* dname);
int readMessage(char *msg);

// clinet initialization code

int client_init(char *argv[])
{
  printf("======= client init ==========\n");

  printf("1 : get server info\n");
  hp = gethostbyname(argv[1]);
  if (hp==0){
     printf("unknown host %s\n", argv[1]);
     exit(1);
  }

  SERVER_IP   = *(long *)hp->h_addr;
  SERVER_PORT = atoi(argv[2]);

  printf("2 : create a TCP socket\n");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock<0){
     printf("socket call failed\n");
     exit(2);
  }

  printf("3 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = SERVER_IP;
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("4 : connecting to server ....\n");
  r = connect(server_sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
     printf("connect failed\n");
     exit(1);
  }

  printf("5 : connected OK to \007\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s  IP=%s  PORT=%d\n", 
          hp->h_name, inet_ntoa(SERVER_IP), SERVER_PORT);
  printf("---------------------------------------------------------\n");

  printf("========= init done ==========\n");
}

main(int argc, char *argv[ ])
{
       //*Lab 5 CODE
  char *tok;
  char cmdArgs[2][MAX] = {'\0'};
  char cwd[128];
  int cmdCount = 0;
       //LAB 5 CODE*

  int n;
  char line[MAX], ans[MAX];

       //*Lab 5 CODE
  getcwd(cwd, 128);   // get CWD pathname
       // Lab 5 CODE*
  printf("cwd: %s\n", cwd);

  if (argc < 3){
     printf("Usage : client ServerName ServerPort\n");
     exit(1);
  }

  client_init(argv);
  // sock <---> server
  printf("********  processing loop  *********\n");
  while (1){
    //printf("input two numbers to add : ");
    //bzero(line, MAX);                // zero out line[ ]

          // *LAB 5 CODE
    printf("Input a command: ");    
          // LAB 5 CODE*

    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
       exit(0);

         // *Lab 5 CODE
    tok = strtok(line, " ");
    cmdCount++;

    while(tok != NULL)
    {
      strcpy(cmdArgs[cmdCount-1], tok);
      tok = strtok(NULL, " ");
      cmdCount++;
    }

    if(strcmp(cmdArgs[0], "lcat") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {mycat(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");}
    }
    else if (strcmp(cmdArgs[0], "lpwd") == 0) 
    {
      mypwd(cwd);
    }
    else if (strcmp(cmdArgs[0], "lls") == 0) 
    {
      printf("args: %s\n", cmdArgs[1]);
      if(strcmp(cmdArgs[1],"") != 0) {myls(cmdArgs[1]);}
      else 
      {
        printf("cwd: %s\n", cwd);
        myls(cwd);
      }
    }
    else if (strcmp(cmdArgs[0], "lcd") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {mycd(cmdArgs[1]);}
      else {mycd(cwd);}
    }
    else if (strcmp(cmdArgs[0], "lmkdir") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {mymkdir(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");}
    }
    else if (strcmp(cmdArgs[0], "lrmdir") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {myrmdir(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");}
    }
    else if (strcmp(cmdArgs[0], "lrm") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {myrm(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");}
    }
    else if (strcmp(cmdArgs[0], "quit") == 0) 
    {
      //CLIENT EXIT
    }
    else // Not a local command, work with the sever
    {
      // Send command to server
      n = write(server_sock, cmdArgs[0], MAX);
      printf("client: wrote n=%d bytes; line=(%s)\n", n, line);
      // Send command pathname
      //n = write(server_sock, cmdArgs[0], MAX);
      //printf("client: wrote n=%d bytes; line=(%s)\n", n, line);

      // Now read the Reply and Results in from the Server...

      //REPLY
      //n = read(server_sock, ans, MAX);
      //printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);

      //RESULTS
      //n = read(server_sock, ans, MAX);
      //printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);

      readMessage(ans);
    }
         // Lab 5 CODE*

    //
    // Send ENTIRE line to server
    //n = write(server_sock, line, MAX);
    //printf("client: wrote n=%d bytes; line=(%s)\n", n, line);

    // Read a line from sock and show it
    //n = read(server_sock, ans, MAX);
    //printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);
  }
}

int mymkdir(char *dname)
{
  if (mkdir("newdir", 0777) < 0){
    printf("errno=%d : %s\n", errno, strerror(errno));}
  return 0;
}

int myrmdir(char *dname)
{
  rmdir(dname);
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
 int myls(char dname[128])
{
  printf("directory name: %s\n", dname);
  struct stat mystat, *sp;
  int r;
  
  sp = &mystat;
  if (r = lstat(dname, sp) < 0){
     printf("no such file %s\n", dname); return(1);
  }
  if (S_ISDIR(sp->st_mode)) {ls_dir(dname);}
  else {ls_file(dname);}
  return 0;
}

int ls_dir(char *dname)
{
  DIR *dp = opendir(dname);
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

void mycd(char* dname)
{
  chdir(dname);
}

int readMessage(char *msg)
{
  // keep reading line from server until EOS is received
  int n = 0;
  n = read(server_sock, msg, MAX);
  while(strcmp(msg, "EOS"))
  {
    printf("%s", msg);
    n = read(server_sock, msg, MAX);
  }
}


