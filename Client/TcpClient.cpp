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


//TODO Find better solution to parse header
#pragma pack(push, 1)
struct ServerResponseHeader
{
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;
};
#pragma pack(pop)

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
		// fatch the size of the payload		
		char responseHeader[sizeof(ServerResponseHeader)];
		boost::asio::read(s, boost::asio::buffer(responseHeader, sizeof(ServerResponseHeader)));

		ServerResponseHeader serverResponseHeader;
		memcpy(&serverResponseHeader, responseHeader, sizeof(ServerResponseHeader));

		char *response = new char[sizeof(ServerResponseHeader) + serverResponseHeader.payloadSize];

		memcpy(response, responseHeader, sizeof(ServerResponseHeader));

		size_t replyLength =
			boost::asio::read(s, boost::asio::buffer(response + sizeof(ServerResponseHeader), serverResponseHeader.payloadSize));

		if (replyLength != serverResponseHeader.payloadSize)
		{
			Logger::error("Get wrong payload size!");
			return NULL;
		}

		return response;
	}
	catch (std::exception& e)
	{
		Logger::error("Exception: " + std::string(e.what()) + "\n");
		return NULL;
	}	
}
