#include "../lib/linuxSocket.hpp"
#include "../lib/defines.hpp"

#include "sys/socket.h"
#include "arpa/inet.h"
#include "iostream"
#include "string.h"
#include "unistd.h"
#include <cerrno>


int linuxSocket::createServerSocket(int port)
{
    struct sockaddr_in addr;
	int sockFd;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
    {
        perror("Server Socket Cannot Created!\n");
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        perror("setsockopt failed\n");
        close(sockFd);
        return (EXIT_FAILURE);
    }


    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sockFd, (struct sockaddr *)&addr, sizeof(sockaddr_in)) < 0) {
        perror("Socket bind failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockFd, MAX_EVENTS) == -1)
    {
        perror("Socket listen failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    return (sockFd);
}