#include "MessageUMenu.h"
#include <iostream>
#include "RSAWrapper.h"
#include "Base64Wrapper.h"
#include <fstream>
#include <boost/uuid/uuid_io.hpp>

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

#pragma pack(push, 1)
struct ClientRequestHeader
{
    boost::uuids::uuid clientId;
    uint8_t version;
    uint16_t code;
    uint32_t payloadSize;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct clientRegisterUserRequestData
{
    ClientRequestHeader clientRequestHeader;
    char name[255];
    char publicKey[RSAPublicWrapper::KEYSIZE];
};
#pragma pack(pop)

union ClientRegisterRequest
{
    clientRegisterUserRequestData data;
    char buffer[sizeof(clientRegisterUserRequestData)];
};


#pragma pack(push, 1)
struct ServerResponseHeader
{
    uint8_t version;
    uint16_t code;
    uint32_t payloadSize;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ServerRegisterUserResponse
{
    ServerResponseHeader serverResponseHeader;
    boost::uuids::uuid clientId;
};
#pragma pack(pop)

void initMessage(char* message, int length)
{
    for (int i = 0; i < length; i++)
        message[i] = '\0';
}

void saveMyInfoFile(const std::string& userName, const std::string& clientId, const std::string& base64PrivateKey)
{
    //TODO Move file name const, maybe #define FILE_NAME("my.info")
    const std::string FILE_NAME = "my.info";

    std::fstream myInfoFile;
    myInfoFile.open(FILE_NAME, std::ios::out);
    if (myInfoFile.is_open())
    {
        myInfoFile << userName << std::endl;
        myInfoFile << clientId << std::endl;
        myInfoFile << base64PrivateKey << std::endl;
        myInfoFile.close();

        std::cout << "Save new user properties to " << FILE_NAME <<  " file success!" << std::endl;
    }
    else
    {
        std::cout << "Can't access to the file " << FILE_NAME << ", save user properties failed!" << std::endl;
        return;
    }
}

bool isMyInfoFileExists()
{
    //TODO Move file name const, maybe #define FILE_NAME("my.info")
    const std::string FILE_NAME = "my.info";

    std::fstream myInfoFile;
    myInfoFile.open(FILE_NAME, std::ios::in);
    if (myInfoFile.is_open())
    {
        std::cout << "File " << FILE_NAME << " already exists, can't register new user!" << std::endl;
        myInfoFile.close();
        return true;
    }
    else
    {
        return false;
    }
}

void MessageUMenu::registerUser()
{
	system("cls");

    if (isMyInfoFileExists())
    {
        system("pause");
        return;
    }        

	std::cout << "Please enter your user name: ";

	std::string userName;
	std::cin >> userName;

    // TODO Should decide what is the max length of each packets
    const int BUFFER_SIZE = 1024;

    // Create client request header and payload

    ClientRegisterRequest clientRequest;
    clientRequest.data.clientRequestHeader.version = 1;
    clientRequest.data.clientRequestHeader.code = 1100;
    clientRequest.data.clientRequestHeader.payloadSize = 415;

    const unsigned short clientRequestSize = sizeof(ClientRegisterRequest);

    // 1. Create an RSA decryptor. this is done here to generate a new private/public key pair
    RSAPrivateWrapper rsaPrivateWrapper;

    // 2. get the public key	
    rsaPrivateWrapper.getPublicKey(clientRequest.data.publicKey, RSAPublicWrapper::KEYSIZE);

    //TODO Init with m.data.name = { 0 }
    initMessage(clientRequest.data.name, 255);
    memcpy(clientRequest.data.name, userName.c_str(), userName.length());

    char requestBuffer[BUFFER_SIZE];

    memcpy(requestBuffer, clientRequest.buffer, clientRequestSize);
    
    // Send client register user request to the server, get response and handle it according to the data
    char* response = tcpClient->sendRequestToServer(requestBuffer);
    if (response == NULL)
    {
        std::cout << "Failed to register new user!" << std::endl;
    }
    else
    {        
        ServerRegisterUserResponse serverRegisterUserResponse;
        memcpy(&serverRegisterUserResponse, response, sizeof(ServerRegisterUserResponse));

        std::cout << "Response data:" << std::endl;
        std::cout << "Version: " << (int)serverRegisterUserResponse.serverResponseHeader.version << ", Code: " << serverRegisterUserResponse.serverResponseHeader.code << ", Payload Size: "
                        << serverRegisterUserResponse.serverResponseHeader.payloadSize << ", ClientId: " << to_string(serverRegisterUserResponse.clientId) << std::endl;
        if (serverRegisterUserResponse.serverResponseHeader.code == 9000)
        {
            std::cout << "Get status code 9000 from the server, failed to register new user!" << std::endl;
        }
        else
        {
            std::cout << "Success to register new user" << std::endl;            

            std::string base64PrivateKeyStr = Base64Wrapper::encode(rsaPrivateWrapper.getPrivateKey());
            saveMyInfoFile(userName, to_string(serverRegisterUserResponse.clientId), base64PrivateKeyStr);
        }

        delete response;
    }

    //TODO Check maybe have other option to pause
    system("pause");    
}