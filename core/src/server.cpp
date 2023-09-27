#include "../lib/server.hpp"
#include "../lib/defines.hpp"
//#include "../lib/fdWatch.hpp"

#include "linuxSocket.hpp"
#include "iostream"
#include "unistd.h"
#include "arpa/inet.h"
#include <csignal>
#include <thread>

static int cycle = 1;

void signalHandler(int signum) {
    std::cout << "SIGINT HANDLING!" << std::endl;
	cycle = 0;
}

server::server()
{
	this->serverFd = -1;
	signal(SIGINT, ::signalHandler);
}

server::~server()
{
	if (this->serverFd != -1)
		close(this->serverFd);
	for(int i = 0; i < MAX_EVENTS; i++)
	{
		if (clients[i].cliFd != -1)
			close(clients[i].cliFd);
	}
}

std::string	server::add_headers(std::string htmlstr, std::string type, int code)
{
	std::string headers;
	headers += "HTTP/1.1 " + std::to_string(code) + " OK\n";
	//headers += "Date: " + get_time_stamp() + "\n";
	headers += "Server: webserv/1.00\n";
	headers += "Content-Type: " + type + "\n";
	headers += "Content-Length: " + std::to_string(htmlstr.size()) + "\n";
	headers += "Connection: close\n";
	std::string res;
	res = headers + "\n" + htmlstr;
	return (res);
}


std::string	addd_headers(std::string htmlstr, std::string type, int code)
{
	std::string headers;
	headers += "HTTP/1.1 " + std::to_string(code) + " OK\n";
	//headers += "Date: " + get_time_stamp() + "\n";
	headers += "Server: webserv/1.00\n";
	headers += "Content-Type: " + type + "\n";
	headers += "Content-Length: " + std::to_string(htmlstr.size()) + "\n";
	headers += "Connection: close\n";
	std::string res;
	res = headers + "\n" + htmlstr;
	return (res);
}

void wwriteEvent(int cliFd, client *clients, std::map<std::string, std::function<std::string(client)> > postFunctions, std::map<std::string, std::function<std::string(client)> > getFunctions)
{
	std::string resp = "404NotFound!";
	
	if (clients[cliFd].method == "POST")
	{
		for (std::map<std::string,std::function<std::string(client)> >::iterator it = postFunctions.begin(); it != postFunctions.end(); it++)
		{
			if ((*it).first == clients[cliFd].dir)
			{
				resp = (*it).second(clients[cliFd]);
				break;
			}
		}
	}
	else if (clients[cliFd].method == "GET")
	{
		for (std::map<std::string,std::function<std::string(client)> >::iterator it = getFunctions.begin(); it != getFunctions.end(); it++)
		{
			if ((*it).first == clients[cliFd].dir)
			{
				resp = (*it).second(clients[cliFd]);
				break;
			}
		}
	}

	resp = addd_headers(resp, "text/html", 200);
	if (write(cliFd, resp.c_str(), resp.length()) == -1)
	{
		std::cout << "Write Err!" << std::endl;
	}
	else
		std::cout << resp << std::endl;

	close(cliFd);
	clients[cliFd].cliFd = -1;

}

void	server::startServer(int port)
{
	this->serverFd = linuxSocket::createServerSocket(port);
	fdWatch whatcher(this->serverFd);
	int eventCount;
	int eventType;
	int cliFd;
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen;

	clientAddrLen = sizeof(clientAddr);
	while (cycle)
	{
		eventCount = whatcher.getEvents();
		for (int i = 0; i < eventCount; i++)
		{
			eventType = whatcher.fdWatchCheckEvent(i);
			if (eventType == NEWCONN)
			{
				cliFd = accept(this->serverFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
				if (cliFd == -1)
				{
					std::cerr << "Connection Failed!" << std::endl;
				}
				else
				{
					std::cout << "New client connected" << std::endl;
					whatcher.fdWatchAddEvent(cliFd, READ);
					clients[cliFd].cliFd = cliFd;
					std::cout << "connection fd " << cliFd << std::endl;
				}
			}
			else
			{
				if (eventType == READ)
					this->readEvent(i, &whatcher);
				else if (eventType == WRITE)
				{
					cliFd = whatcher.fdWatchGetFd(i);
					
					std::thread threadObj(wwriteEvent, cliFd, clients, postFunctions, getFunctions);
					threadObj.detach();
					whatcher.fdWatchDestroyEvent(cliFd);
				}
				else
				{
					std::cout << "Close Conn!" << std::endl;
					cliFd = whatcher.fdWatchGetFd(i);
					whatcher.fdWatchDestroyEvent(cliFd);
					close(cliFd);
					clients[cliFd].cliFd = -1;
				}
			}
		}
	}
}

void server::readEvent(int i, fdWatch *whatcher)
{
	int cliFd;

	cliFd = whatcher->fdWatchGetFd(i);
	char buffer[1024];
	int bytesRead = recv(cliFd, buffer, sizeof(buffer), 0);
	if (bytesRead <= 0) {
		whatcher->fdWatchDestroyEvent(cliFd);
		close(cliFd);
		clients[cliFd].cliFd = -1;
		std::cout << "Client disconnected" << std::endl;
	}
	else
	{
		buffer[bytesRead] = '\0';
		clients[cliFd].reqParser(buffer);
		whatcher->fdWatchChangeEvent(cliFd, WRITE);
		std::cout << "Received data from client: " << buffer << std::endl;
	}
}




void server::handlePost(std::string dir, std::function<std::string(client)> func)
{
    this->postFunctions.insert(std::make_pair(dir, func));
}

void server::handleGet(std::string dir, std::function<std::string(client)> func)
{
    this->getFunctions.insert(std::make_pair(dir, func));
}