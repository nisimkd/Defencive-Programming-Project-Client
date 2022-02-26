#include "TcpClient.h"
#include <iostream>
#include <boost/asio.hpp>
#include "Logger.h"
using boost::asio::ip::tcp;

TcpClient::TcpClient(const std::string& serverAddress, const std::string& serverPort)
{
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
}

TcpClient::~TcpClient()
{

}

char* TcpClient::sendRequestToServer(const char* request)
{
	try
	{
		//TODO Move socket to class member
		// Create tcp socket
		boost::asio::io_context ioContext;
		tcp::socket s(ioContext);
		tcp::resolver resolver(ioContext);

		// Connect to the server by server address and server port
		boost::asio::connect(s, resolver.resolve(serverAddress, serverPort));

		// Send requset to the server
		boost::asio::write(s, boost::asio::buffer(request, BUFFER_SIZE));

		// Get response from the server
		char* response = new char[BUFFER_SIZE];
		size_t reply_length =
			boost::asio::read(s, boost::asio::buffer(response, BUFFER_SIZE));

		return response;
	}
	catch (std::exception& e)
	{
		Logger::error("Exception: " + std::string(e.what()) + "\n");
		return NULL;
	}
}