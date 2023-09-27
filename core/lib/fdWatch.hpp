#pragma once


#include "../lib/defines.hpp"
#include "arpa/inet.h"

#ifdef __linux__

#include "sys/epoll.h"

class fdWatch
{
	public:
		fdWatch(int serverFd);
		fdWatch();
		int getEvents();
		void connectionHandler();
		int	fdWatchCheckEvent(int index);
		int fdWatchGetFd(int index);
		void fdWatchAddEvent(int fd, int type);
		void fdWatchChangeEvent(int fd, int type);
		void fdWatchDestroyEvent(int fd);
		void *fdWatchGetData(int index);
		//variables
		int serverFd;
		int	epollFd;
		epoll_event event;
		epoll_event events[MAX_EVENTS];
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen;
		int cliFd;
};
#endif