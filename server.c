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
	int client = accept(sock_conn, 0, 0);

	char buff[BUFF_SIZE] = { 0 };
	// receive client request
	ssize_t n = recv(client,buff,BUFF_SIZE,0);
	if(n < 0) 
	{
		perror("Read error");
		exit(1);
	}
	// check if message is complete, this implementation a complete message ends with '\n'
	int finished = 0;
	int offset = n;
	if(finished)
	{
		// check if agreed upon '\n' deliminating end of message is within buffer
		for(ssize_t i = 0; i < n; ++i)
		{
			if(buff[i] == '\n')
			{
				finished = 1;
				break;
			}
			
		}
		n = recv(sock_conn, buff + offset, BUFF_SIZE - offset, 0);
		offset += n;
	}
	return 0;
}
