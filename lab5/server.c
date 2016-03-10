// This is the echo SERVER server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
       n = read(client_sock, cmd, MAX); //read the cmd sent from client
       m = read(client_sock, pathname, MAX); // read the pathname sent from client
       if (n==0){
           printf("server: client died, server loops\n");
           close(client_sock);
           break;
      }
      
      // show the client passes
      printf("server: read  n=%d bytes; cmd=[%s]\n", n, cmd);
      printf("server: read  m=%d bytes; pathname=[%s]\n", m, pathname);
      
      strcat(cmd, " ");
      strcat(pathname, " ");

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
          strcpy(line, "ERROR: pathname not given...");}

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
          strcpy(line, "ERROR: pathname not given...");}

          n = write(client_sock, line, MAX);
          printf("server: wrote n=%d bytes; result=[%s]\n", n, line);
          printf("server: ready for next request\n");
        }
          
      }
      else if(strcmp(cmd, "get") == 0)
      {
        //TODO ADD CALL FOR GET WHEN THE FUNCTION IS WRITTEN
      }
      else if(strcmp(cmd, "put") == 0)
      {
        //TODO ADD CALL FOR PUT WHEN THE FUNCTION IS WRITTEN
      }
      else 
      {
        printf("Client passed an improper command...");
        strcpy(line, "Client passed an improper command...");

        n = write(client_sock, line, MAX);
        printf("server: wrote n=%d bytes; result=[%s]\n", n, line);
        printf("server: ready for next request\n");
      }


    }
 }
}

