#include <iostream> /* cout, endl */
#include <unistd.h> /* read */
#include <stdio.h> /* perror */
#include <arpa/inet.h> /* send, connect */

#define Q_LEN (10)
#define PORT (4651)
#define BUFF_SIZE (30)
#define STDIN 0

namespace
{
/* update family, address and port */
void UpdateAddressIMP(struct sockaddr_in *addr, 
                      short family, 
                      unsigned long s_addr, 
                      unsigned short port);

/* create socket and bind to address */
void CreateAndBindSocketIMP(int *sock, 
                            short family, 
                            __socket_type type, 
                            int proto,
                            struct sockaddr_in *addr);
/* uses cin and prints a key was pressed */
void StdinHandlerIMP();

/* read buffer from udp client and send acknowledgement */
void UdpHandlerIMP(int socket, struct sockaddr_in *addr/*, fd_set *readfds*/);

/* read buffer from tcp client and send acknowledgement */
void TcpHandlerIMP(int socket, 
                   struct sockaddr_in *addr, 
                   fd_set *readfds,
                   int *maxfd);

/* read buffer from bc client and send acknowledgement */
void BcHandlerIMP(int socket, struct sockaddr_in *addr/*, fd_set *readfds*/);

/* create an array of sockets for bc,tcp,udp and STDIN */
void SetSockInArrayIMP(int *arr, size_t arr_len, fd_set *readfds);

/* return number of highest socket num */
int GetMaxSocketIMP(int *arr, size_t arr_len);

/* close all sockets */
void CloseSocketsIMP(int *arr, size_t arr_len);

/* main function of client handling */
void HandleDifferentClients(fd_set *readfds, 
							int *sock_arr, 
							struct sockaddr_in *server_array, 
							int sock_arr_len);
}

int main()
{
    struct sockaddr_in non_bc_server_addr;
	struct sockaddr_in bc_server_addr;
	struct sockaddr_in server_array[] = {non_bc_server_addr, bc_server_addr};
    int sockfd_bc = 0;
    int sockfd_tcp = 0;
    int sockfd_udp = 0;
    int sock_arr[] = {sockfd_bc, sockfd_udp, sockfd_tcp, STDIN};
    size_t len = sizeof(sock_arr) / sizeof(sock_arr[0]);
    int broadcast = 1;
    fd_set readfds;

    UpdateAddressIMP(&server_array[0], AF_INET, INADDR_LOOPBACK, PORT);
    UpdateAddressIMP(&server_array[1], AF_INET, INADDR_BROADCAST, PORT);

    CreateAndBindSocketIMP(&sock_arr[0], 
                           AF_INET, 
                           SOCK_DGRAM, 
                           IPPROTO_UDP, 
                           &server_array[1]);

    CreateAndBindSocketIMP(&sock_arr[1], 
                           AF_INET, 
                           SOCK_DGRAM, 
                           IPPROTO_UDP, 
                           &server_array[0]);

    CreateAndBindSocketIMP(&sock_arr[2],
                           AF_INET, 
                           SOCK_STREAM, 
                           IPPROTO_TCP, 
                           &server_array[0]);
   
    if (-1 == setsockopt(sock_arr[0], SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    {
        perror("setsockopt");
    }

    SetSockInArrayIMP(sock_arr, len, &readfds);

	if (-1 == listen(sock_arr[2], Q_LEN))
    {
        perror("listen");
    }

 
	HandleDifferentClients(&readfds, sock_arr, server_array, len);

    std::cout << "outside main loop\n";
    CloseSocketsIMP(sock_arr, len);
}

namespace
{
void HandleDifferentClients(fd_set *readfds, 
							int *sock_arr, 
							struct sockaddr_in *server_array, 
							int sock_arr_len)
{
	int maxfd = GetMaxSocketIMP(sock_arr, sock_arr_len);
	while (1)
    {
		fd_set temp_set = *readfds;
        if (-1 == select(maxfd + 1, &temp_set, NULL, NULL, NULL))
        {
            perror("select: ");
        }
	//	std::cout << "after select select\n";
        for (int j = 0; j < maxfd + 1; ++j)
        {
            if (FD_ISSET(j, &temp_set))
            {
                if (j == STDIN)
                {
                    StdinHandlerIMP();
                }

                if (j == sock_arr[0])
                {
                    BcHandlerIMP(sock_arr[0], &server_array[1]);  
                }
                    
                if (j == sock_arr[2])
                {
                    TcpHandlerIMP(sock_arr[2], &server_array[0], readfds, &maxfd); 
                }
                if (j == sock_arr[1])
                {
                    UdpHandlerIMP(sock_arr[1], &server_array[0]);
                }
				else
				{
					struct sockaddr_in addr;
					char read_buff[100];
					char send_buff[] = "hello";
					read(j, read_buff, BUFF_SIZE);
					std::cout << read_buff << std::endl;

					if (-1 == sendto(j, send_buff, 
									BUFF_SIZE, MSG_CONFIRM, 
									(struct sockaddr *)&addr, 
									sizeof(addr)))
					{
						perror("server else sendto");
					}
				}
            } // end of -> if (FD_ISSET(i, &temp_set))			
        }        
    }
}

void UpdateAddressIMP(struct sockaddr_in *addr, 
                      short family, 
                      unsigned long s_addr, 
                      unsigned short port)
{
    addr->sin_family = family;
    addr->sin_addr.s_addr = htonl(s_addr);
    addr->sin_port = htons(port);
}

void CreateAndBindSocketIMP(int *sock, 
                            short family, 
                            __socket_type type, 
                            int proto,
                            struct sockaddr_in *addr)
{
    *sock = socket(family, type, proto);
    if (-1 == *sock)
    {
        perror("socket: ");
    }
    if (-1 == bind(*sock, (struct sockaddr *)addr, sizeof(*addr)))
    {
        perror("bind: ");
    }
}                            

void StdinHandlerIMP()
{
   std::string s;
    std::cin >> s;
    std::cout << "A key was pressed" << std::endl;
}

void UdpHandlerIMP(int socket, struct sockaddr_in *addr)
{
    std::cout << "sockfd_udp" << std::endl;
    socklen_t size = sizeof(*addr);
    char read_buff[BUFF_SIZE];
    char send_buff[] = "server send udp reply";

    recvfrom(socket, read_buff, BUFF_SIZE, 0, (struct sockaddr *)addr, &size);
    std::cout << read_buff << std::endl;

    if (-1 == sendto(socket, send_buff, 
                     BUFF_SIZE, MSG_CONFIRM,
                     (struct sockaddr *)addr, 
                     size))
    {
        perror("server bc sendto");
    }
}

void TcpHandlerIMP(int socket, 
                   struct sockaddr_in *addr, 
                   fd_set *readfds,
                   int *maxfd)
{
    std::cout << "sockfd_tcp" << std::endl;
    socklen_t size = sizeof(*addr);
    char read_buff[BUFF_SIZE];
    char send_buff[] = "Server send tcp reply";
    
    int new_tcp_sock = accept(socket, (struct sockaddr *)addr, &size);
	FD_SET(new_tcp_sock, readfds);
   
    if (*maxfd < new_tcp_sock)
    {
        *maxfd = new_tcp_sock;
    }

    read(new_tcp_sock, read_buff, BUFF_SIZE);
    std::cout << read_buff << std::endl;

    if (-1 == sendto(new_tcp_sock, send_buff, 
                     BUFF_SIZE, MSG_CONFIRM, 
                     (struct sockaddr *)addr, 
                     size))
    {
        perror("server tcp sendto");
    }
}

void BcHandlerIMP(int socket, struct sockaddr_in *addr)
{
    std::cout << "sockfd_bc" << std::endl;
    socklen_t size = sizeof(*addr);
    char read_buff[BUFF_SIZE];
    char send_buff[] = "server send bc reply";

    recvfrom(socket, read_buff, BUFF_SIZE, 0, (struct sockaddr *)addr, &size);
    std::cout << read_buff << std::endl;

    if (-1 == sendto(socket, send_buff, 
                     BUFF_SIZE, MSG_CONFIRM, 
                     (struct sockaddr *)addr, 
                     size))
    {
        perror("server bc sendto");
    }
}

void SetSockInArrayIMP(int *arr, size_t arr_len, fd_set *readfds)
{
    for (size_t i = 0; i < arr_len; ++i)
    {
        FD_SET(arr[i], readfds);
    }
}

int GetMaxSocketIMP(int *arr, size_t arr_len)
{
    int max_socket = 0;

    for (size_t i = 0; i < arr_len; ++i)
    {
        if (max_socket < arr[i])
        {
            max_socket = arr[i];
        }
    }

    return max_socket;
}

void CloseSocketsIMP(int *arr, size_t arr_len)
{
    for (size_t i = 0 ; i < arr_len - 1; ++i)
    {
        close(arr[i]);
    }
}
}