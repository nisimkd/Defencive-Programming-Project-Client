#include "TcpClient.h"
#include "RSAWrapper.h"
#include "Base64Wrapper.h"

#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>

//TODO Remove unused includes

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
	//TODO Put it in better place
	const int BUFFER_SIZE = 1024;

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
		char *response = new char[BUFFER_SIZE];
		size_t reply_length =
			boost::asio::read(s, boost::asio::buffer(response, BUFFER_SIZE));				

		return response;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		return NULL;
	}	
}