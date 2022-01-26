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

#pragma pack(push, 1)
struct clientMessageData
{
	char clientId[16];
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;

	char name[255];
};
#pragma pack(pop)

union ClientMessage
{
	clientMessageData data;
	char buffer[sizeof(clientMessageData)];
};

#pragma pack(push, 1)
struct serverMessageData
{
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;

	char response[255];
};
#pragma pack(pop)

void initMessage(char message[], int length);

std::string sendRequestToServer(std::string serverAddress, std::string serverPort, std::string messageRequest)
{
	const int max_length = 1024;

	ClientMessage m;
	m.data.version = 1;
	m.data.code = 1100;
	m.data.payloadSize = 255;
	
	initMessage(m.data.name, 255);
	memcpy(m.data.name, messageRequest.c_str(), messageRequest.length());
	
	char buf[16 + sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint32_t) + 255];

	memcpy(buf, m.buffer, 16 + sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint32_t) + 255);

	try
	{
		boost::asio::io_context ioContext;
		tcp::socket s(ioContext);
		tcp::resolver resolver(ioContext);
		boost::asio::connect(s, resolver.resolve(serverAddress, serverPort));

		for (; ; ) 
		{			
			boost::asio::write(s, boost::asio::buffer(buf, 1024));
			char reply[max_length];
			serverMessageData serverMessage;
			size_t reply_length =
				boost::asio::read(s, boost::asio::buffer(reply, max_length));

			memcpy(&serverMessage, reply, sizeof(serverMessageData));

			std::cout << "Version: " << (int)serverMessage.version << ", Code: " << serverMessage.code << ", Payload Size: " << serverMessage.payloadSize << ", Response: " << serverMessage.response << std::endl;

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