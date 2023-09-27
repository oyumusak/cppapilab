#pragma once


#include "client.hpp"
#include "vector"
# include <functional>
# include "../lib/defines.hpp"
# include "../lib/fdWatch.hpp"


class server
{
	public:
		server();
		~server();
		int serverFd;
		client clients[1024];
		void	startServer(int port);
		std::string	add_headers(std::string htmlstr, std::string type, int code);
        void handlePost(std::string dir, std::function<std::string(client)> func);
        void handleGet(std::string dir, std::function<std::string(client)> func);
		std::map<std::string, std::function<std::string(client)> > postFunctions;
        std::map<std::string, std::function<std::string(client)> > getFunctions;
		void readEvent(int i, fdWatch *whatcher);
		void writeEvent(int i, fdWatch *whatcher);
};