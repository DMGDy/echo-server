#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

int
main(void)
{
	int sock_conn = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr = {
		AF_INET,
		htons(8080),
		inet_addr("127.0.0.1")
	};
	
	if(connect(sock_conn, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		perror("Error connecting");
		exit(1);
	}

	// read string from user
	char* buff = NULL;
	size_t b = 0;
	size_t n = getline(&buff, &b, stdin);
	if(n == -1)
	{
		perror("Error reading stdin");
		exit(2);
	}

	ssize_t sent = send(sock_conn, buff, n, 0);
	if(sent < 0)
	{
		perror("Error sending");
		exit(3);
	}
	
	// if less bytes sent than length of buffer, not all sent
	int finished = 1;
	if(sent != n)
	{
		finished = 0;
	}

	printf("Sending %zu bytes\n", n);
	printf("finished == %d\n", finished);
	// send until the sent is equal to the buffer size (n)
	while(!finished)
	{
		printf("here\n");
		sent += send(sock_conn, buff + sent, n - sent, 0);
		printf("sent: %zd, n: %zd", sent, n);
		if(sent == n)
		{
			finished = 1;
			break;
		}
	
	}
	printf("%zd bytes of data have been sent\n", sent);

	// expect the same message back
	char rec_buff[BUFF_SIZE] = { 0 };
	
	ssize_t rec_size = recv(sock_conn, rec_buff, BUFF_SIZE, 0);
	if(rec_size < 0)
	{
		perror("Read error");
		exit(4);
	}
	
	finished = 0;
	while(!finished)
	{
		for(ssize_t i = 0; i < rec_size; ++i)
		{
			if(rec_buff[i] == '\n')
			{
				finished = 1;
				break;
			}
			if(!finished)
			{
				ssize_t n_rec = recv(sock_conn,
							rec_buff + rec_size,
							BUFF_SIZE - rec_size,
							0
						     );
				// error to be handled here
				rec_size += n_rec;
			}
		}
	}
		printf("%zd bytes received back with message:\n%s", rec_size, rec_buff);
	
	close(sock_conn);
	free(buff);

	return 0;
}
