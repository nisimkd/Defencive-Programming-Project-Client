#include <iostream>
#include "TcpClient.h"

#pragma once
class MessageUMenu
{
private:
	TcpClient *tcpClient;
	int getInput();
	void displayMainMenuOptions();
	void registerUser();
public:
	MessageUMenu(const std::string&, const std::string&);
	~MessageUMenu();
	// Constants declarations
	enum mainMenuOptions
	{
		exit = 0,
		user_register = 110,
		request_clients_list = 120,
		request_public_key = 130,
		request_waiting_messages = 140,
		send_text_message = 150,
		request_symmetric_key = 151,
		send_symmetric_key = 152,
	};	
	int runMainMenu();
};