#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>

int main(int argc, char** argv){
	
	//Setting up socket file descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd<0){
		printf("There was an error creating the socket\n");
		return 1;
	}
	
	//Array of bits, each bit is a flag for 1 file descriptor
	fd_set readfds;
	//Struct for a timeout
	struct timeval timeout;


	//Setting up socket's address struct
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(9876);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	//Adding an empty socket address store the client's later
	struct sockaddr_in client;
	
	//Binding the socket to the address
	if(bind(sockfd,(struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
		printf("\nError Binding\n");
		return 1;
	}
	printf("Binding Done\n");
	
	//Lisiting on it with a que of 3
	listen(sockfd,3);

	printf("Waiting for incomming connections\n");
	int c = sizeof(struct sockaddr_in);

	//Accepting connection from client
	int e = accept(sockfd, (struct sockaddr *)&client, (socklen_t*)&c);
	if(c < 0){
		printf("Accepting failed\n");
		return 1;
	}
	printf("Client Accepted\n");

	int read_size;
	char line[5000];
	char line2[5000];
	int sret, ret;
	while((read_size = recv(e, line, 5000, 0)) > 0){
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		sret = select(8,&readfds,NULL,NULL,&timeout);

		if(sret == 0){
			printf("sret = %d\n", sret);
			printf(" timeout\n");
		}else{

			printf("Line Received: %s",line);
			printf("Enter a response: ");
			fgets(line2, 5000, stdin);
			write(e, line2, strlen(line));
			memset(line, 0, sizeof(line));
			memset(line2, 0, sizeof(line2));
		}
	}
}

