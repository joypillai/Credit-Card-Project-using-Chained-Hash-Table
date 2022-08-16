#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include "Card.h"
#define PORT 8080

int main(int argc, char const* argv[])
{
	int sock = 0, valread,i=0,j=0;
	struct sockaddr_in serv_addr;
	char *buffline;
	char *buff;
	struct Card hello;
	struct Card newptr;
	buffline=malloc(sizeof(char)*400);
	buff=malloc(sizeof(char)*400);
	if(buff== NULL || buffline == NULL){
		fprintf(stderr,"Error in assigning pointer!");
		exit(1);
	}
	char buffer[1024] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	ssize_t readbytes;
	int fd = open ("data.txt", O_CREAT|O_RDONLY,S_IRWXU);

	if (fd < 0)
	{
		perror ("Undable to Open file OPEN faied\n");
		exit (1);
	}
	printf("All Data Send to the Server\n");

	while((readbytes=read(fd,&newptr,sizeof(newptr)))>0){
	  hello=newptr;
	  send(sock,&hello,sizeof(hello),0);
	  sleep(0.8);
	}

	/*while((readbytes = read(fd, buff,sizeof(buff))) > 0 ){

		i=0;

		for(i=0;i<readbytes;i++){
			if(buff[i]=='\n'){
				buffline[j]=buff[i];
				send(sock,buffline,sizeof(buffline),0);
				memset(buffline,0,400);
				sleep(0.8);
			}
			else{
				buffline[j]=buff[i];
				j++;
			}
		}
	}*/
		return 0;
	
}
