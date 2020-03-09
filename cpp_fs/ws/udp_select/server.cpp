#include <sys/socket.h> 
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring> /* memset */
#include <arpa/inet.h>

#define PROTOCOL (0)
#define Q_LEN (5)
#define PORT (4651)
#define BUFF_SIZE (30)
#define STDIN 0

int main()
{
    struct sockaddr_in server_addr1, server_addr2;
    int broadcast = 1;
    fd_set readfds;
	char read_buff[256];
    
    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    
    server_addr1.sin_family = AF_INET;
    server_addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr1.sin_port = htons(PORT);

	server_addr2.sin_family = AF_INET;FD_SET(STDIN, &readfds);
    server_addr2.sin_addr.s_addr = INADDR_BROADCAST;
    server_addr2.sin_port = htons(PORT);

    int sockfd_bc = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);
    if (-1 == sockfd_bc)
    {
        perror("socket bc: ");
    }

    int sockfd_udp = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);
    if (-1 == sockfd_udp)
    {
        perror("socket udp: ");
    }

    int sockfd_tcp = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
    if (-1 == sockfd_tcp)
    {
        perror("socket tcp: ");
    }
    
    if (-1 == setsockopt(sockfd_bc, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    {
        perror("setsockopt");
    }

	if (-1 == bind(sockfd_bc, (struct sockaddr *)&server_addr2, sizeof(server_addr2)))
	{
		perror("bind sockfd_bc");
	}
	if (-1 == bind(sockfd_tcp, (struct sockaddr *)&server_addr1, sizeof(server_addr1)))
	{
		perror("bind sockfd_tcp");
	}
	if (-1 == bind(sockfd_udp, (struct sockaddr *)&server_addr1, sizeof(server_addr1)))
	{
		perror("bind sockfd_udp");
	}

    FD_SET(STDIN, &readfds);
    FD_SET(sockfd_bc, &readfds);
    FD_SET(sockfd_tcp, &readfds);
    FD_SET(sockfd_udp, &readfds);

	if (-1 == listen(sockfd_tcp, Q_LEN))
    {
        perror("listen");
    }

    for (int i = 0; i < 30 ; ++i)
    {
     //   sleep(10);

        fd_set temp_set = readfds;
		int sel = select(sockfd_tcp + 1, &temp_set, NULL, NULL, &tv);
        if (-1 == sel)
        {
            perror("select: ");
        }

        for (int i = 0; i < sockfd_tcp + 1; ++i)
        {
            if (FD_ISSET(i, &temp_set))
            {
                if (0 == i)
                {
                    std::cout << "A key was pressed" << std::endl;
                }

                if (i == sockfd_bc)
                {
					socklen_t size = 0;
                    std::cout << "sockfd_bc" << std::endl;
                    char buff[] = "about to recieve bc";
					recvfrom(sockfd_bc, read_buff, 256, 0, (struct sockaddr *)&server_addr1, &size);
					std::cout << read_buff << std::endl;

                    if (-1 == sendto(sockfd_bc, buff, 
                                        BUFF_SIZE, MSG_CONFIRM, 
                                        (struct sockaddr *)&server_addr2, 
                                        sizeof(server_addr2)))
                    {
                        perror("server bc sendto");
                    }
                }
                    
                if (i == sockfd_tcp)
                {
					socklen_t size = sizeof(server_addr1);
                    std::cout << "sockfd_tcp" << std::endl;
					int new_tcp_sock = accept(sockfd_tcp, (struct sockaddr *)&server_addr1, &size);
                    char buff[] = "about to recieve tcp";
					std::cout << read_buff << std::endl;

					read(new_tcp_sock, read_buff, 256);
                    if (-1 == sendto(new_tcp_sock, buff, 
                                        BUFF_SIZE, MSG_CONFIRM, 
                                        (struct sockaddr *)&server_addr1, 
                                        sizeof(server_addr1)))
                    {
                        perror("server tcp sendto");
                    }
                }
                if (i == sockfd_udp)
                {
                    std::cout << "inside server about to receive udp" << std::endl; 
                    char buff[] = "server sends reply to udp";
					socklen_t size = 0;

					recvfrom(sockfd_udp, read_buff, 256, 0, (struct sockaddr *)&server_addr1, &size);
					std::cout << read_buff << std::endl;
                    if (-1 == sendto(sockfd_udp, buff, 
                                     BUFF_SIZE, MSG_CONFIRM, 
                                     (struct sockaddr *)&server_addr1, 
                                     sizeof(server_addr1)))
                    {
                        perror("server udp sendto");
                    }

					FD_CLR(sockfd_udp, &readfds);
                }
            } // end of -> if (FD_ISSET(i, &temp_set))
        }        
    }

    close(sockfd_bc);
    close(sockfd_udp);
    close(sockfd_tcp);
}