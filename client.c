#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/un.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define size 1024
#define PORT 9925

char rbuf[size], wbuf[size];
int client_fd, ret, bytes, ret;
struct sockaddr_in client_addrobj;
void * thread_function(void *);
void *retval;
pthread_t thread_id;


int main()
{
	

/************************************ Socket *********************************************/

	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd == -1)
		perror("Socket creation failed !!!\n");
	else
		printf("Socket created successfully \n\n");

/*********************************** Naming the socket *********************************************/

	client_addrobj.sin_family = AF_INET;
	client_addrobj.sin_port = htons(PORT);
	client_addrobj.sin_addr.s_addr = inet_addr("127.0.0.1");


/*********************************** Connect *********************************************/
	ret = connect(client_fd, (struct sockaddr *)&client_addrobj, sizeof(client_addrobj) );

	if(ret == -1)
		perror("Connection failed !!!\n");
	else
		printf("Connection established successfully \n\n");

/******************************** Conversation **********************************************/

		bzero(wbuf, size);//strcpy(wbuf, "\0" );
		bzero(wbuf, size);//strcpy(rbuf, "\0" );

		ret = pthread_create(&thread_id, NULL, thread_function, NULL);
		while(strncmp(wbuf, "end", 3) != 0 )
		{
			strcpy(wbuf, "\0" );
			printf("\nEnter the message from client....\n");
			fgets(wbuf, size, stdin);
			bytes = send(client_fd, wbuf, sizeof(wbuf), 0);

			if(bytes == -1)
				perror("Writing by the server failed!!!\n");
			else
				printf("         Message sent           \n");
		}
		
		pthread_join(thread_id, &retval);
		printf("\n***************** Conversation Over *******************\n");
		
		

	close(client_fd);

return 0;
}


void * thread_function(void *arg)
{
	strcpy(rbuf, "\0" );
	while( strncmp(rbuf, "end", 3) != 0 )
	{
		strcpy(rbuf, "\0" );
		bytes = recv(client_fd, rbuf, size, 0);
		if(bytes == -1)
			perror("Reading from server failed!!!\n");
		else
			printf("\nYou got a message from server :\n  %s \n", rbuf);
	}
	
	pthread_exit(NULL);
}
