// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include "commands.c" //LAB 5 CODE

#define MAX 256

//run after running server.c
//run with arguments IP and Port from server.c

// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 

int server_sock, r;
int SERVER_IP, SERVER_PORT; 


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
  char cmdArgs[2][MAX];
  char cwd[128];
  int cmdCount = 0;
       //LAB 5 CODE*

  int n;
  char line[MAX], ans[MAX];

       //*Lab 5 CODE
  getcwd(cwd, 128);   // get CWD pathname
       // Lab 5 CODE*

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
      cmdArgs[0] = tok;
      tok = strtok(NULL, " ");
      cmdCount++;
    }

    if(strcmp(cmdArgs[0], "lcat") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {mycat(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");
    }
    else if (strcmp(cmdArgs[0], "lpwd") == 0) 
    {
      mypwd(cwd);
    }
    else if (strcmp(cmdArgs[0], "lls") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {myls(cmdArgs[1]);}
      else {myls(cwd);}
    }
    else if (strcmp(cmdArgs[0], "lcd") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {mycd(cmdArgs[1]);}
      else {mycd(cwd);}
    }
    else if (strcmp(cmdArgs[0], "lmkdir") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {mymkdir(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");
    }
    else if (strcmp(cmdArgs[0], "lrmdir") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {myrmdir(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");
    }
    else if (strcmp(cmdArgs[0], "lrm") == 0) 
    {
      if(strcmp(cmdArgs[1],"") != 0) {myrm(cmdArgs[1]);}
      else {printf("ERROR: pathname not given");
    }
    else // Not a local command, work with the sever
    {
      // Send command to server
      n = write(server_sock, cmdArgs[0], MAX);
      printf("client: wrote n=%d bytes; line=(%s)\n", n, line);
      // Send command pathname
      n = write(server_sock, cmdArgs[0], MAX);
      printf("client: wrote n=%d bytes; line=(%s)\n", n, line);

      // Now read the Reply and Results in from the Server...

      //REPLY
      n = read(server_sock, ans, MAX);
      printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);

      //RESULTS
      n = read(server_sock, ans, MAX);
      printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);
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

