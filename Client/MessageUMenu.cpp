#include "MessageUMenu.h"
#include <iostream>

MessageUMenu::MessageUMenu(const std::string& serverAddress, const std::string& serverPort)
{
    this->tcpClient = new TcpClient(serverAddress, serverPort);
}

MessageUMenu::~MessageUMenu()
{
    delete tcpClient;
}

int MessageUMenu::runMainMenu()
{
    MessageUMenu::mainMenuOptions userSelection;

    do
    {
        system("cls");

        MessageUMenu::displayMainMenuOptions();

        userSelection = (MessageUMenu::mainMenuOptions)MessageUMenu::getInput();

        switch (userSelection)
        {
        case MessageUMenu::mainMenuOptions::user_register:
        {
            registerUser();
            break;
        }
        case MessageUMenu::mainMenuOptions::request_clients_list:
            break;
        case MessageUMenu::mainMenuOptions::request_public_key:
            break;
        case MessageUMenu::mainMenuOptions::request_waiting_messages:
            break;
        case MessageUMenu::mainMenuOptions::send_text_message:
            break;
        case MessageUMenu::mainMenuOptions::request_symmetric_key:
            break;
        case MessageUMenu::mainMenuOptions::send_symmetric_key:
            break;
        case MessageUMenu::mainMenuOptions::exit:
            break;
        default:
            break;
        }
    } while (userSelection != MessageUMenu::mainMenuOptions::exit);

    return EXIT_SUCCESS;
}

int MessageUMenu::getInput()
{
    int choise;
    std::cin >> choise;
    return choise;
}

void MessageUMenu::displayMainMenuOptions()
{
    std::cout << "MessageU client at your service." << std::endl << std::endl;
    std::cout << "110) Register" << std::endl;
    std::cout << "120) Request for clients list" << std::endl;
    std::cout << "130) Request for public key" << std::endl;
    std::cout << "140) Request for waiting messages" << std::endl;
    std::cout << "150) Send a text message" << std::endl;
    std::cout << "151) Send a request for symmetric key" << std::endl;
    std::cout << "152) Send your symmetric key" << std::endl;
    std::cout << "0) Exit client" << std::endl;
}

void MessageUMenu::registerUser()
{
	system("cls");

	std::cout << "Please enter your user name: ";

	std::string userName;
	std::cin >> userName;
    
    tcpClient->sendRequestToServer(userName);

    //TODO Check maybe have other option to pause
    system("pause");    
}


