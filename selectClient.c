#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <sys/select.h>


int main(int argc, char** argv){

	//Socket File Descriptor
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd<0){
		printf("There was an error creating the socket\n");
		return 1;
	}

	//Setting up the Socket
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(9876);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	if(connect(sockfd,(struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
		printf("Connection has failed\n");
		return 1;
	}
	printf("Connection established\n");

	char line[5000];
	char line2[5000];
	while(1){
		printf("Enter a message: ");
		//scanf("%s", line);
		fgets(line, 5000, stdin);

		if(send(sockfd, line, strlen(line), 0) < 0){
			printf("\nSending has failed\n");
			return 1;
		}

		if(recv(sockfd, line2, 5000, 0) < 0){
			printf("\nReceiving has failed\n");
			break;
		}

		printf("Server Reply: %s", line2);
		memset(line, 0, sizeof(line));
		memset(line2, 0, sizeof(line2));
	}
	close(sockfd);
	return 0;

/*	if(e<0){
		//No one is listening 
		printf("There was an error connecting to the server\n");
		return 2;
	}

	printf("Enter a line: ");
	char line[5000];
	char line2[5000];

	//Getting the line from the user using fgets
	fgets(line, 5000, stdin);

	//sending the data 
	send(sockfd, line, strlen(line) + 1,0);

	//Receiving form the server
	recv(sockfd, line2, 5000, 0);

	//printing received line
	printf("\nLine Received: %s\n", line2); 


	return 0; */
}
