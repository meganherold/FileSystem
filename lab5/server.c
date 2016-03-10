// This is the echo SERVER server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netdb.h>
#include "commands.h" //LAB 5 CODE
#define  MAX 256

// Define variables:
struct sockaddr_in  server_addr, client_addr, name_addr;
struct hostent *hp;

int  mysock, client_sock;              // socket descriptors
int  serverPort;                     // server port number
int  r, length, n, m;                   // help variables

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int mymkdir (char *dirname);
int myrmdir(char *dirname);
int myrm(char *filename);
int mycat(char *filename);
int mycp(char *file1, char *file2);
int myls(char *dirname);
int ls_dir(char *dirname);
int ls_file(char *fname);
void mypwd();
void mycd(char* dname);
int sendData(const char *line, ...);

// Server initialization code:

int server_init(char *name)
{
   printf("==================== server init ======================\n");   
   // get DOT name and IP address of this host

   printf("1 : get and show server host info\n");
   hp = gethostbyname(name);
   if (hp == 0){
      printf("unknown host\n");
      exit(1);
   }
   printf("    hostname=%s  IP=%s\n",
               hp->h_name,  inet_ntoa(*(long *)hp->h_addr));
  
   //  create a TCP socket by socket() syscall
   printf("2 : create a socket\n");
   mysock = socket(AF_INET, SOCK_STREAM, 0);
   if (mysock < 0){
      printf("socket call failed\n");
      exit(2);
   }

   printf("3 : fill server_addr with host IP and PORT# info\n");
   // initialize the server_addr structure
   server_addr.sin_family = AF_INET;                  // for TCP/IP
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // THIS HOST IP address  
   server_addr.sin_port = 0;   // let kernel assign port

   printf("4 : bind socket to host info\n");
   // bind syscall: bind the socket to server_addr info
   r = bind(mysock,(struct sockaddr *)&server_addr, sizeof(server_addr));
   if (r < 0){
       printf("bind failed\n");
       exit(3);
   }

   printf("5 : find out Kernel assigned PORT# and show it\n");
   // find out socket port number (assigned by kernel)
   length = sizeof(name_addr);
   r = getsockname(mysock, (struct sockaddr *)&name_addr, &length);
   if (r < 0){
      printf("get socketname error\n");
      exit(4);
   }

   // show port number
   serverPort = ntohs(name_addr.sin_port);   // convert to host ushort
   printf("    Port=%d\n", serverPort);

   // listen at port with a max. queue of 5 (waiting clients) 
   printf("5 : server is listening ....\n");
   listen(mysock, 5);
   printf("===================== init done =======================\n");
}


main(int argc, char *argv[])
{
   char *hostname;
   char line[MAX];
   char *snum1, *snum2;
   int num1, num2, result;
   size_t sz;
   char cmd[MAX], pathname[MAX];
   char cwd[128];
   char *tok;
   char cmdArgs[2][MAX] = {0};
   int cmdCount = 0;

   getcwd(cwd, 128);   // get CWD pathname

   if (argc < 2)
      hostname = "localhost";
   else
      hostname = argv[1];

   
 
   server_init(hostname); 

   // Try to accept a client request
   while(1){
     printf("server: accepting new connection ....\n"); 

     // Try to accept a client connection as descriptor newsock
     length = sizeof(client_addr);
     client_sock = accept(mysock, (struct sockaddr *)&client_addr, &length);
     if (client_sock < 0){
        printf("server: accept error\n");
        exit(1);
     }
     printf("server: accepted a client connection from\n");
     printf("-----------------------------------------------\n");
     printf("        IP=%s  port=%d\n", inet_ntoa(client_addr.sin_addr.s_addr),
                                        ntohs(client_addr.sin_port));
     printf("-----------------------------------------------\n");

     // Processing loop: newsock <----> client
     while(1){
       n = read(client_sock, line, MAX); //read the cmd sent from client
       //m = read(client_sock, pathname, MAX); // read the pathname sent from client
       if (n==0){
           printf("server: client died, server loops\n");
           close(client_sock);
           break;
      }
      
      // show the client passes
      printf("server: read  n=%d bytes; line=[%s]\n", n, line);
      
      //strcat(cmd, " ");
      //strcat(pathname, " ");
      tok = strtok(line, " ");
      cmdCount++;

      while(tok != NULL)
      {
        strcpy(cmdArgs[cmdCount-1], tok);
        tok = strtok(NULL, " ");
        cmdCount++;
      }
      strcpy(cmd, cmdArgs[0]);
      strcpy(pathname, cmdArgs[1]);

      if(strcmp(cmd, "pwd") == 0)
      {
        mypwd(cwd);
      }
      else if(strcmp(cmd, "ls") == 0)
      {
        if(strcmp(pathname,"") != 0) {myls(pathname);}
        else {myls(cwd);}
      }
      else if(strcmp(cmd, "cd") == 0)
      {
        if(strcmp(pathname,"") != 0) {mycd(pathname);}
        else {mycd(cwd);}
      }
      else if(strcmp(cmd, "mkdir") == 0)
      {
        if(strcmp(pathname,"") != 0) {mymkdir(pathname);}
        else 
        {
          printf("ERROR: pathname not given by client");
          strcpy(line, "ERROR: pathname not given...");

          n = write(client_sock, line, MAX);
          printf("server: wrote n=%d bytes; result=[%s]\n", n, line);
          printf("server: ready for next request\n");
        }
      }
      else if(strcmp(cmd, "rmdir") == 0)
      {
        if(strcmp(pathname,"") != 0) {myrmdir(pathname);}
        else 
        {
          printf("ERROR: pathname not given by client");
          strcpy(line, "ERROR: pathname not given...");

          n = write(client_sock, line, MAX);
          printf("server: wrote n=%d bytes; result=[%s]\n", n, line);
          printf("server: ready for next request\n");
        }
      }
      else if(strcmp(cmd, "rm") == 0)
      {
        if(strcmp(pathname,"") != 0) {myrm(pathname);}
        else 
        {
          printf("ERROR: pathname not given by client");
          strcpy(line, "ERROR: pathname not given...");

          n = write(client_sock, line, MAX);
          printf("server: wrote n=%d bytes; result=[%s]\n", n, line);
          printf("server: ready for next request\n");
        }
          
      }
     
      else if(strcmp(cmd, "put") == 0)
      {
        if(strcmp(pathname,"") != 0) {mycp(pathname, cwd);}
        else 
        {
          printf("ERROR: pathname not given by client");
          strcpy(line, "ERROR: pathname not given...");

          n = write(client_sock, line, MAX);
          printf("server: wrote n=%d bytes; result=[%s]\n", n, line);
          printf("server: ready for next request\n");
        }
      }
      else 
      {
        printf("Client passed an improper command...%s", cmd);
        strcpy(line, "Client passed an improper command...");

        n = write(client_sock, line, MAX);
        printf("server: wrote n=%d bytes; result=[%s]\n", n, line);
        printf("server: ready for next request\n");
      }


    }
 }
}

int mymkdir(char *dirname)
{
  if (mkdir("newdir", 0777) < 0){
    printf("errno=%d : %s\n", errno, strerror(errno));}
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
    sendData("%s\n", d->d_name);
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
     sendData("can't stat %s\n", fname); 
     exit(1);
  }

  if ((sp->st_mode & 0xF000) == 0x8000)
     sendData("%c",'-');
  if ((sp->st_mode & 0xF000) == 0x4000)
     sendData("%c",'d');
  if ((sp->st_mode & 0xF000) == 0xA000)
     sendData("%c",'l');

  for (i=8; i >= 0; i--){
    if (sp->st_mode & (1 << i))
	sendData("%c", t1[i]);
    else
	sendData("%c", t2[i]);
  }

  printf("%4d ",sp->st_nlink);
  printf("%4d ",sp->st_gid);
  printf("%4d ",sp->st_uid);
  printf("%8d ",sp->st_size);

  // print time
  strcpy(ftime, ctime(&sp->st_ctime));
  ftime[strlen(ftime)-1] = 0;
  sendData("%s  ",ftime);

  // print name
  sendData("%s", basename(fname));  

  // print -> linkname if it's a symbolic file
  if ((sp->st_mode & 0xF000)== 0xA000){ // YOU FINISH THIS PART
     // use readlink() SYSCALL to read the linkname
     readlink(fname, linkname, 64);
     sendData(" -> %s", linkname);
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

int sendData(const char *line, ...)
{
  char message[MAX];
  va_list args;
  va_start(args, line);
  vsprintf(message, line, args);

  // send the echo line to client 
  n = write(client_sock, message, MAX);
  printf("sent: %s", message);

  va_end(args);
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

