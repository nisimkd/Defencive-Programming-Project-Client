#include <iostream>
#include "TcpClient.h"
#include "UserManager.h"

#pragma once
class ConsoleUI
{
private:
	// Constants declarations
	enum class mainMenuOptions
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

	UserManager* userManager;

	int getInput();
	void displayMainMenuOptions();
public:
	ConsoleUI(const std::string&, const std::string&);
	ConsoleUI(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);	
	~ConsoleUI();
	int run();
};