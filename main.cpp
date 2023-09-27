#include "core/lib/cppapi.hpp"

std::string omer(client cliData)
{
	return ("hello world");
}

std::string selim(client cliData)
{
	if (cliData.ipAddr == "127.0.0.1")
		return ("Acces denied");
	return (cliData.ipAddr);
}

int	main()
{
	server mysw;

	mysw.handleGet("/", omer);
	mysw.handleGet("/selim", selim);

	mysw.startServer(2625);

	return (0);
}