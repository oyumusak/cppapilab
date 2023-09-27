#include "../lib/fdWatch.hpp"

#include "iostream"


#ifdef __linux__

#include "unistd.h"

fdWatch::fdWatch(int serverFd)
{
	this->serverFd = serverFd;


	epollFd = epoll_create1(0);
	if (epollFd == -1)
	{
		close(this->serverFd);
		std::cerr << "EpollFd Error!" << std::endl;
		exit(-1);
	}

	event.events = EPOLLIN | EPOLLHUP;
	event.data.fd = this->serverFd;


	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, this->serverFd, &event) == -1) {
        perror("Epoll control failed");
        close(this->serverFd);
		close(epollFd);
		exit(-1);
    }
}


int	fdWatch::getEvents()
{
	return (epoll_wait(this->epollFd, this->events, MAX_EVENTS, -1));
}

int	fdWatch::fdWatchCheckEvent(int index)
{
	if (events[index].data.fd == this->serverFd)
	{
		return (NEWCONN);
	}
	else if (events[index].events & EPOLLIN)
	{
		return (READ);
	}
	else if (events[index].events & EPOLLOUT)
	{
		return (WRITE);
	}
	return (-1);
}

int	fdWatch::fdWatchGetFd(int index)
{
	return (this->events[index].data.fd);
}

void fdWatch::fdWatchAddEvent(int fd, int type)
{
	if (type == READ)
    	event.events = EPOLLIN  | EPOLLHUP;
	else if (type == WRITE)
		event.events = EPOLLOUT | EPOLLHUP;
    event.data.fd = fd;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
}

void fdWatch::fdWatchChangeEvent(int fd, int type)
{
	if (type == WRITE)
		event.events = EPOLLOUT | EPOLLHUP;
	else if (type == READ)
		event.events = EPOLLIN  | EPOLLHUP;

	event.data.fd = fd;
	epoll_ctl(epollFd, EPOLL_CTL_MOD, event.data.fd, &event);
}

void fdWatch::fdWatchDestroyEvent(int fd)
{
	epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, &event);
}


void	*fdWatch::fdWatchGetData(int index)
{
	return (events[index].data.ptr);
}
#endif