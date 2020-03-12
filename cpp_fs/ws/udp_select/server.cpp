#include <iostream> /* cout, endl */
#include <unistd.h> /* read */
#include <stdio.h> /* perror */
#include <cstring> /* memset */
#include <cassert> /* assert */
#include <arpa/inet.h> /* send, connect */
#include <stdexcept> /* std::runtime_error*/	

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
void UDPandOpenTcpHandler(int socket, fd_set *readfds);

/* read buffer from tcp client and send acknowledgement */
void TcpHandlerIMP(int socket, 
                   struct sockaddr_in *addr, 
                   fd_set *readfds,
                   int *maxfd);

/* read buffer from bc client and send acknowledgement */
void BcHandlerIMP(int socket);

/* create an array of sockets for bc,tcp,udp and STDIN */
void SetSockInArrayIMP(int *arr, size_t arr_len, fd_set *readfds);

/* return number of highest socket num */
int GetMaxSocketIMP(int *arr, size_t arr_len);

/* close all sockets */
void CloseSocketsIMP(fd_set *readfds, size_t maxSockId);

/* main function of client handling */
void HandleDifferentClients(fd_set *readfds, 
							int *sock_arr, 
							struct sockaddr_in *server_array, 
							int sock_arr_len,
							int *maxSockId);
}

int main()
{
    struct sockaddr_in non_bc_server_addr;
	struct sockaddr_in bc_server_addr;
	memset(&non_bc_server_addr, 0, sizeof(non_bc_server_addr));
	memset(&bc_server_addr, 0, sizeof(non_bc_server_addr));
	struct sockaddr_in server_array[] = {non_bc_server_addr, bc_server_addr};
    int sockfd_bc = 0;
    int sockfd_tcp = 0;
    int sockfd_udp = 0;
    int sock_arr[] = {sockfd_bc, sockfd_udp, sockfd_tcp, STDIN};
    size_t len = sizeof(sock_arr) / sizeof(sock_arr[0]);
    int broadcast = 1;
    fd_set readfds;
	int maxSockId = 0;

    UpdateAddressIMP(&server_array[0], AF_INET, INADDR_LOOPBACK, PORT);
    UpdateAddressIMP(&server_array[1], AF_INET, INADDR_BROADCAST, PORT);

	try
	{
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
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}   
   
    if (-1 == setsockopt(sock_arr[0], SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)))
    {
        perror("setsockopt");
		throw std::runtime_error("setseckopt failed");
    }

    SetSockInArrayIMP(sock_arr, len, &readfds);

	if (-1 == listen(sock_arr[2], Q_LEN))
    {
        perror("listen");
		throw std::runtime_error("listen failed");
    }
	try
	{
		HandleDifferentClients(&readfds, sock_arr, server_array, len, &maxSockId);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	CloseSocketsIMP(&readfds, maxSockId);

	return 0;
}

namespace
{
void HandleDifferentClients(fd_set *readfds,
							int *sock_arr, 
							struct sockaddr_in *server_array,
							int sock_arr_len,
							int *maxSockId)
{
	assert(readfds);
	assert(sock_arr);
	assert(server_array);
	assert(sock_arr_len > 0);

	*maxSockId = GetMaxSocketIMP(sock_arr, sock_arr_len);
	while (true)
    {
		fd_set temp_set = *readfds;
        if (-1 == select(*maxSockId + 1, &temp_set, NULL, NULL, NULL))
        {
            perror("select: ");
			throw std::runtime_error("select failed");
        }
        for (int j = 0; j < *maxSockId + 1; ++j)
        {
            if (FD_ISSET(j, &temp_set))
            {
                if (j == STDIN)
                {
                    StdinHandlerIMP();
                }

                else if (j == sock_arr[0])
                {
                    BcHandlerIMP(j);  
                }
                    
                else if (j == sock_arr[2])
                {
                    TcpHandlerIMP(j, &server_array[0], readfds, maxSockId); 
                }

				else	// UDP + open TCP
				{
					UDPandOpenTcpHandler(j, readfds);
				}
            } 
        }        
    }
}

void UpdateAddressIMP(struct sockaddr_in *addr, 
                      short family, 
                      unsigned long s_addr, 
                      unsigned short port)
{
	assert(addr);

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
	assert(sock);
	assert(addr);

    *sock = socket(family, type, proto);
    if (-1 == *sock)
    {
        perror("socket: ");
		throw std::runtime_error("socket failed");
    }
    if (-1 == bind(*sock, (struct sockaddr *)addr, sizeof(*addr)))
    {
        perror("bind: ");
		throw std::runtime_error("bind failed");
    }
}                            

void StdinHandlerIMP()
{
   std::string s;
    std::cin >> s;
    std::cout << "A key was pressed" << std::endl;
}

void UDPandOpenTcpHandler(int socket, fd_set *readfds)
{
	assert(readfds);
	assert(socket >= 0);

    std::cout << "sockfd reply" << std::endl;
	struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    char read_buff[BUFF_SIZE];
    char send_buff[] = "server send a reply";

    ssize_t bytes = recvfrom(socket, read_buff, BUFF_SIZE, 0, (struct sockaddr *)&addr, &size);
	if (0 < bytes)
	{
    	std::cout << read_buff << std::endl;

		if (-1 == sendto(socket, send_buff, 
						BUFF_SIZE, MSG_CONFIRM,
						(struct sockaddr *)&addr, 
						size))
		{
			perror("server sendto");
			throw std::runtime_error("sendto failed");
		}
	}
	else if (0 == bytes)
	{
		FD_CLR(socket, readfds);
	}
	else
	{
		perror("server recvfrom");
		throw std::runtime_error("recvfrom failed");
	}
}

void TcpHandlerIMP(int socket, 
                   struct sockaddr_in *addr, 
                   fd_set *readfds,
                   int *maxfd)
{
	assert(socket >= 0);
	assert(addr);
	assert(readfds);

    std::cout << "sockfd tcp accept" << std::endl;
    socklen_t size = sizeof(*addr);
    
    int new_tcp_sock = accept(socket, (struct sockaddr *)addr, &size);
	if (-1 == new_tcp_sock)
	{
		perror("accept: ");
		throw std::runtime_error("accept failed");
	}

	FD_SET(new_tcp_sock, readfds);
   
    if (*maxfd < new_tcp_sock)
    {
        *maxfd = new_tcp_sock;
    }
}

void BcHandlerIMP(int socket)
{
	assert(socket >= 0);

    std::cout << "sockfd_bc" << std::endl;
	struct sockaddr_in addr;
    socklen_t size = sizeof(addr);
    char read_buff[BUFF_SIZE];
    char send_buff[] = "server send bc reply";

    recvfrom(socket, read_buff, BUFF_SIZE, 0, (struct sockaddr *)&addr, &size);
    std::cout << read_buff << std::endl;

    if (-1 == sendto(socket, send_buff, 
                     BUFF_SIZE, MSG_CONFIRM, 
                     (struct sockaddr *)&addr, 
                     size))
    {
        perror("server bc sendto");
    }
}

void SetSockInArrayIMP(int *arr, size_t arr_len, fd_set *readfds)
{
	assert(arr);
	assert(readfds);

	FD_ZERO(readfds);
    for (size_t i = 0; i < arr_len; ++i)
    {
        FD_SET(arr[i], readfds);
    }
}

int GetMaxSocketIMP(int *arr, size_t arr_len)
{
	assert(arr);

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

void CloseSocketsIMP(fd_set *readfds, size_t maxSockId)
{
	assert(readfds);
	for (size_t i = 0 ; i < maxSockId ; ++i)
    {
		if (FD_ISSET(i, readfds))
		{
			close(i);
		}
    }
}
}