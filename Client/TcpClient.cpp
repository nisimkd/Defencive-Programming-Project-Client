#include "TcpClient.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
/*
TcpClient::TcpClient(std::string serverAddress, std::string serverPort)
{
	this->tcpSocket = new tcp::socket(this->ioContext);
	this->tcpResolver = new tcp::resolver(this->ioContext);
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
}
*/

void initMessage(char message[], int length);

std::string sendRequestToServer(std::string serverAddress, std::string serverPort, std::string messageRequest)
{
	const int max_length = 1024;

	try
	{
		boost::asio::io_context ioContext;
		tcp::socket s(ioContext);
		tcp::resolver resolver(ioContext);
		boost::asio::connect(s, resolver.resolve(serverAddress, serverPort));

		for (; ; ) 
		{
			char request[max_length];
			initMessage(request, max_length);
			memcpy(request, messageRequest.c_str(), messageRequest.length());
			boost::asio::write(s, boost::asio::buffer(request, max_length));
			char reply[max_length];
			size_t reply_length =
				boost::asio::read(s, boost::asio::buffer(reply, max_length));

			std::cout << "Reply is: " << reply << "\n";

			return std::string(reply);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}

void initMessage(char message[], int length)
{
	for (int i = 0; i < length; i++)
		message[i] = '\0';
}