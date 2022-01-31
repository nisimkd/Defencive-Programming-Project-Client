#include <iostream>
#pragma once

class TcpClient
{
private:	
	std::string serverAddress;
	std::string serverPort;		
public:
	TcpClient(const std::string&, const std::string&);
	~TcpClient();
	char* sendRequestToServer(const char*);
};

