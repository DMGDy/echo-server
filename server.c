/*
 * echo-server-example
 *
 * Copyright (C) 2025 Dylan Dy <dylangarza1909@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// Also, this was all written with the ed text editor, not some bloated software like vim or neovim

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 1048

int
main(void)
{
	struct sockaddr_in addr = {
		AF_INET,
		htons(8080),
		INADDR_ANY
	};

	int sock_conn = socket(AF_INET,SOCK_STREAM,0);
	bind(sock_conn, (struct sockaddr*)&addr, sizeof(addr));

	// 8 max connections to be waiting
	listen(sock_conn, 8);
	printf("Listening with INADDR_ANY with port 8080...\n");
	int client = accept(sock_conn, 0, 0);
	while(1)
	{
		printf("New connection accepted\n");
	
		char buff[BUFF_SIZE] = { 0 };
		// receive client request
		ssize_t n = recv(client,buff,BUFF_SIZE,0);
		printf("Received a message\n");
		printf("%s",buff);
		if(n < 0) 
		{
			perror("Read error");
			exit(1);
		}
		// check if message is complete, this implementation a complete message ends with '\n'
		int finished = 0;
		int size = n;
		while(!finished)
		{
			// check if agreed upon '\n' deliminating end of message is within buffer
			for(ssize_t i = 0; i < size; ++i)
			{
				if(buff[i] == '\n')
				{
					finished = 1;
					break;
				}
				
			}
			if(finished)
			{
				break;
			}
	
			n = recv(sock_conn, buff + size, BUFF_SIZE - size, 0);
			size += n;
	
		}
		// now that message is 100% received (as long as '\n' was sent), send message back to echo
		printf("Received %zd bytes with following message: %s", size, buff);
		printf("Now echoing message back...\n");
		n = send(client, buff, size, 0);
		if(n < 0)
		{
			perror("Send error");
			exit(2);
		}
		
		if(n < size)
		{
			finished = 0;
		}
	
		// if not finished sending all data, keep sending, increasing size until done
		size = n;
		while(!finished)
		{
			n = send(sock_conn, buff + size, size, 0);
			if(n == 0) 
			{
				finished = 1;
				break;
			}
			size += n;
		}
		printf("%zd bytes sent\n", size);
	
	}

	close(sock_conn);

	close(client);
	printf("Closing server\n");
	return 0;
}
