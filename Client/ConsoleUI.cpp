#include "ConsoleUI.h"
#include <iostream>
#include "RSAWrapper.h"
#include "Base64Wrapper.h"
#include <fstream>

ConsoleUI::ConsoleUI(const std::string& serverAddress, const std::string& serverPort)
{
    this->userManager = new UserManager(serverAddress, serverPort);
}

ConsoleUI::ConsoleUI(const std::string& serverAddress, const std::string& serverPort, const std::string& userName, const std::string& clientId, const std::string& privateKey)
{
    this->userManager = new UserManager(serverAddress, serverPort, userName, clientId, privateKey);
}

ConsoleUI::~ConsoleUI()
{
    delete userManager;
}

int ConsoleUI::run()
{
    ConsoleUI::mainMenuOptions userSelection;



    do
    {
        system("cls");

        ConsoleUI::displayMainMenuOptions();

        userSelection = (ConsoleUI::mainMenuOptions)ConsoleUI::getInput();
        
        //TODO Handle too large user name
        //TODO Handle user selection error
        switch (userSelection)
        {
        case ConsoleUI::mainMenuOptions::user_register:
        {
            system("cls");

            std::cout << "Please enter your user name: ";

            std::string userName;
            std::cin >> userName;

            std::string registerUserResult = userManager->registerUser(userName);

            std::cout << registerUserResult << std::endl;      

            break;
        }
        case ConsoleUI::mainMenuOptions::request_clients_list:
        {
            system("cls");
            std::string clientsListRequestResult = userManager->requestClientsListFromServer();
            std::cout << clientsListRequestResult << std::endl;
            break;
        }
        case ConsoleUI::mainMenuOptions::request_public_key:
        {
            system("cls");

            std::cout << "Please enter the user name of the requested public key: ";

            std::string requestedPublicKeyUserName;
            std::cin >> requestedPublicKeyUserName;

            std::string requestPublicKeyResult = userManager->requestPublicKey(requestedPublicKeyUserName);

            std::cout << requestPublicKeyResult << std::endl;

            break;
        }
        case ConsoleUI::mainMenuOptions::request_waiting_messages:
        {
            system("cls");

            std::string requestWaitingMessagesResult = userManager->requestWaitingMessages();
            std::cout << requestWaitingMessagesResult << std::endl;
            break;
        }            
        case ConsoleUI::mainMenuOptions::send_text_message:
        {
            system("cls");

            std::cout << "Please enter the user name of the user you want send the message: ";
            std::string userName;
            std::cin >> userName;

            std::cout << "Please enter the message: ";
            std::string message;
            std::cin >> message;


            std::string sendMessagesResult = userManager->sendMessage(userName, message);
            std::cout << sendMessagesResult << std::endl;
            break;
        }            
        case ConsoleUI::mainMenuOptions::request_symmetric_key:
        {
            system("cls");

            std::cout << "Please enter the user name of the user you want send the symmetric key resquest: ";
            std::string userName;
            std::cin >> userName;


            std::string requestSymmetricKeyResult = userManager->requestSymmetricKey(userName);
            std::cout << requestSymmetricKeyResult << std::endl;
            break;
        }
            break;
        case ConsoleUI::mainMenuOptions::send_symmetric_key:
        {
            system("cls");

            std::cout << "Please enter the user name of the user you want send the symmetric key: ";
            std::string userName;
            std::cin >> userName;


            std::string sendSymmetricKeyResult = userManager->sendSymmetricKey(userName);
            std::cout << sendSymmetricKeyResult << std::endl;
            break;
        }
        case ConsoleUI::mainMenuOptions::exit:
            break;
        default:
            continue;
            break;
        }

        system("pause");
    } while (userSelection != ConsoleUI::mainMenuOptions::exit);

    return EXIT_SUCCESS;
}

int ConsoleUI::getInput()
{
    int choise;
    std::cin >> choise;
    return choise;
}

void ConsoleUI::displayMainMenuOptions()
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