#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void sendRequestToServer(std::string, std::string, std::string);
#pragma once
/*
class TcpClient
{
private:
	std::string serverAddress;
	std::string serverPort;
	boost::asio::io_context ioContext;
	tcp::socket *tcpSocket;
	tcp::resolver *tcpResolver;

	void sendRequestToServer(std::string);
	void initMessage(char message[], int length);
public:
	TcpClient(std::string, std::string);
};
*/
