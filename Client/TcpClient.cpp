#include "TcpClient.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include "RSAWrapper.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>   
#include "Base64Wrapper.h"

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
	char publicKey[RSAPublicWrapper::KEYSIZE];
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

	boost::uuids::uuid clientId;
};
#pragma pack(pop)

void initMessage(char message[], int length);
void saveMyInfoFile(const std::string& userName, const std::string& clientId, const std::string& base64PrivateKey);

void sendRequestToServer(std::string serverAddress, std::string serverPort, std::string messageRequest)
{
	// TODO Should decide what is the max length of each packets
	const int max_length = 1024;

	ClientMessage m;
	m.data.version = 1;
	m.data.code = 1100;
	m.data.payloadSize = 415;
	
	//TODO Init with m.data.name = { 0 }
	initMessage(m.data.name, 255);
	memcpy(m.data.name, messageRequest.c_str(), messageRequest.length());

	// 1. Create an RSA decryptor. this is done here to generate a new private/public key pair
	RSAPrivateWrapper rsapriv;

	// 2. get the public key	
	rsapriv.getPublicKey(m.data.publicKey, RSAPublicWrapper::KEYSIZE);	// you can get it as a char* buffer

	Base64Wrapper base64Wrapper;

	std::string base64key = Base64Wrapper::encode(rsapriv.getPrivateKey());

	const USHORT clientMessageSize = sizeof(clientMessageData);
	
	char buf[max_length];

	memcpy(buf, m.buffer, clientMessageSize);

	try
	{
		boost::asio::io_context ioContext;
		tcp::socket s(ioContext);
		tcp::resolver resolver(ioContext);
		boost::asio::connect(s, resolver.resolve(serverAddress, serverPort));	
		boost::asio::write(s, boost::asio::buffer(buf, max_length));
		char reply[max_length];
		serverMessageData serverMessage;
		size_t reply_length =
			boost::asio::read(s, boost::asio::buffer(reply, max_length));
			

		memcpy(&serverMessage, reply, sizeof(serverMessageData));

		std::cout << "Version: " << (int)serverMessage.version << ", Code: " << serverMessage.code << ", Payload Size: " << serverMessage.payloadSize << ", ClientId: " << to_string(serverMessage.clientId) << std::endl;

		saveMyInfoFile(messageRequest, to_string(serverMessage.clientId), base64key);
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

void saveMyInfoFile(const std::string& userName, const std::string& clientId, const std::string& base64PrivateKey)
{
	//TODO Move file name const, maybe #define FILE_NAME("my.info")

	const std::string FILE_NAME = "my.info";

	std::fstream myInfoFile;
	myInfoFile.open(FILE_NAME, std::ios::in);
	if (myInfoFile.is_open())
	{
		std::cout << "File " << FILE_NAME << " already exists, can't register new user!" << std::endl;		
		return;
	}
	else
	{
		myInfoFile.close();

		myInfoFile.open(FILE_NAME, std::ios::out);
		if (myInfoFile.is_open())
		{
			myInfoFile << userName << std::endl;
			myInfoFile << clientId << std::endl;
			myInfoFile << base64PrivateKey << std::endl;
			myInfoFile.close();

			std::cout << "Register user success!" << std::endl;
		}
		else
		{
			std::cout << "Can't open the file " << FILE_NAME << ", register new user failed!" << std::endl;
			return;
		}
	}
}