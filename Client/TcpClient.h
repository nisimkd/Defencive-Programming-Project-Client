#include <iostream>
#pragma once

class TcpClient
{
private:
	std::string serverAddress;
	std::string serverPort;	
	void initMessage(char *, int);
	void saveMyInfoFile(const std::string&, const std::string&, const std::string&);
public:
	TcpClient(const std::string&, const std::string&);
	~TcpClient();
	void sendRequestToServer(const std::string&);
};

