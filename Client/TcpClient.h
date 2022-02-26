#include <iostream>
#pragma once

class TcpClient
{
//TODO Put it in better place
#define BUFFER_SIZE 8096
private:	
	std::string serverAddress;
	std::string serverPort;
public:
	TcpClient(const std::string&, const std::string&);
	~TcpClient();
	char* sendRequestToServer(const char*);
};