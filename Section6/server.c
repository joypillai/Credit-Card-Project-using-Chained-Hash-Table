#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include "Card.h"
#include<string.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;

	struct sockaddr_in address;
	struct Card hello;
	struct Card newptr;    
	int opt = 1,retval,error;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char bufferold[1024] = { 0 };
	//char* hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
			== 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
				sizeof(address))
			< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	printf("All Data is send to the Client\n");
	int i=0;
	while(1)
	{
		valread = read(new_socket, buffer, 1024);
		sleep(0.8);
		if(strcmp(buffer,bufferold)==0){
			printf("Exiting.....\n");	
			return 0;
		}
		printf("%s\n", buffer);
		strcpy(bufferold,buffer);

		error = 0;
		socklen_t len = sizeof (error);
		retval = getsockopt (server_fd, SOL_SOCKET, SO_ERROR, &error, &len);

		if (retval != 0) {
			/* there was a problem getting the error code */
			fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
			return 0;
		}

		if (error != 0) {
			/* socket has a non zero error status */
			fprintf(stderr, "socket error: %s\n", strerror(error));
			return 0;
		}
	}
	return 0;
}
