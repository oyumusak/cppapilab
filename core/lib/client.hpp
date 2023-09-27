#pragma once

# include "map"
# include "iostream"


class client
{
    public:
		client();
        void	reqParser(char *req);
        int cliFd;
        std::string req;
        std::string method;
        std::string dir;
        std::string queryString;
        std::string protocol;
        std::string serverName;
        std::string ipAddr;
        std::map<std::string, std::string>	headers;
        int postLen;
        std::string postVal;
        int status;
};