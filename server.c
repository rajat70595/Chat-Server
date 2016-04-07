#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<sys/un.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define size 1024
#define PORT 9925

char wbuf[size], rbuf[size];
void * thread_function(void *);
void *retval;
pthread_t thread_id;
int server_fd, c_fd, ret, c_len, bytes, ret;
struct sockaddr_in server_addrobj, c_addrobj;


int main()
{
	int true = 1;
/*********************************** Unlink ************************************************/

	unlink("SOCKET SERVER");

/*********************************** Socket ***********************************************/

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int));
	if(server_fd == -1)
		perror("Socket creation failed!!!\n");
	else
		printf("Socket created successfully\n");

/*********************************** Bind *************************************************/

	server_addrobj.sin_family = AF_INET;
	server_addrobj.sin_port = htons(PORT);
	server_addrobj.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(server_fd, (const struct sockaddr *)&server_addrobj, sizeof(server_addrobj));
	if(ret==-1)
		perror("Binding failed !!!!\n");
	else
		printf("Binding Successfull\n");

/********************************** Listen *************************************************/

	ret = listen(server_fd, 5);
	if(ret==-1)
		perror("Listening failed!!!\n");
	else
		printf("Listening successfull\n");

/*********************************** Accept ************************************************/

	
	while(1)
	{
		bzero(rbuf, size);//strcpy(rbuf, "\0" );
		bzero(wbuf, size);//strcpy(wbuf, "\0" );

		printf("\nWaiting for client request.......\n");
		c_len=sizeof(c_addrobj);
		//int flags = fcntl(socket, F_GETFL, 0);
		//fcntl(socket, F_SETFL, O_NONBLOCK|flags);
		c_fd = accept(server_fd, (struct sockaddr *)&c_addrobj, &c_len);

		if(c_fd == -1)
			perror("Accept system call failed!!!\n");
		else
			printf("Connection established successfully \n");

/******************************** Conversation **********************************************/




		ret = pthread_create(&thread_id, NULL, thread_function, NULL);
		while(strncmp(wbuf, "end", 3) != 0 )
		{
			strcpy(wbuf, "\0" );
			printf("\nEnter the message from server....\n");
			fgets(wbuf, size, stdin);
			bytes = send(c_fd, wbuf, sizeof(wbuf), 0);

			if(bytes == -1)
				perror("Writing by the server failed!!!\n");
			else
				printf("        Message sent      \n");
		}
		
		pthread_join(thread_id, &retval);
		printf("\n***************** Conversation Over *******************\n");

		
		close(c_fd);
	}

	close(server_fd);
return 0;
}





void * thread_function(void *arg)
{
	bzero(rbuf, size);//strcpy(rbuf, "\0" );
	while( strncmp(rbuf, "end", 3) != 0 )
	{
		bzero(rbuf, size);//strcpy(rbuf, "\0" );
		bytes = recv(c_fd, rbuf, size, 0);
		if(bytes == -1)
			perror("Reading from client failed!!!\n");
		else
			printf("\nYou got a message from client :\n  %s \n", rbuf);
	}
	
	pthread_exit(NULL);
}
