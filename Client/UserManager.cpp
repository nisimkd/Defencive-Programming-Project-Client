#include "UserManager.h"
#include <fstream>
#include "Base64Wrapper.h"
#include <boost/uuid/uuid_io.hpp>
#include "Logger.h"

#pragma region Constructors and Destructors

UserManager::UserManager(const std::string& serverAddress, const std::string& serverPort)
{
    this->tcpClient = new TcpClient(serverAddress, serverPort);
    this->clientId = boost::uuids::uuid();
    this->rsaPrivateWrapper = new RSAPrivateWrapper;
    this->version = VERSION;
}

UserManager::~UserManager()
{
    delete rsaPrivateWrapper;
}

#pragma endregion

#pragma region Public Methods

std::string UserManager::registerUser(const std::string& userName)
{    
    if (UserManager::isMyInfoFileExists())
    {
        return "File " + FILE_NAME + " already exists, can't register new user!\n";
    }

    std::string registerUserResultStr;

    // Create client request header and payload

    char requestBuffer[BUFFER_SIZE];    
    createClientUserRequestBuffer(requestBuffer);

    // Send client register user request to the server, get response and handle it according to the data
    char* response = tcpClient->sendRequestToServer(requestBuffer);
    if (response == NULL)
    {
        registerUserResultStr = "Failed access to the server, failed to register new user, please try again later!\n";
    }
    else
    {
        ServerRegisterUserResponse serverRegisterUserResponse;
        memcpy(&serverRegisterUserResponse, response, sizeof(ServerRegisterUserResponse));

        std::string responseLogMessage = "Response data: Version: " + std::to_string((int)serverRegisterUserResponse.serverResponseHeader.version) + ", Code: " +
            std::to_string(serverRegisterUserResponse.serverResponseHeader.code) + ", Payload Size: " + std::to_string(serverRegisterUserResponse.serverResponseHeader.payloadSize)
            + ", ClientId: " + to_string(serverRegisterUserResponse.clientId) + "\n";
        Logger::info(responseLogMessage);

        if (serverRegisterUserResponse.serverResponseHeader.code == 9000)
        {
            registerUserResultStr = "Server error, failed to register new user, please try again\n";
        }
        else
        {
            registerUserResultStr = "Register new user success!\n";

            std::string base64PrivateKeyStr = Base64Wrapper::encode(rsaPrivateWrapper->getPrivateKey());
            registerUserResultStr += saveMyInfoFile(userName, to_string(serverRegisterUserResponse.clientId), base64PrivateKeyStr);
        }

        delete response;
    }

    return registerUserResultStr;
}

#pragma endregion

#pragma region Private Methods

void UserManager::createClientUserRequestBuffer(char *clientRegisterRequseBuffer)
{
    ClientRegisterRequest clientRegisterRequest;
    clientRegisterRequest.data.header.version = version;
    clientRegisterRequest.data.header.code = UserManager::requestCode::user_register;
    clientRegisterRequest.data.header.payloadSize = sizeof(ClientRegisterUserRequestPayload);
    rsaPrivateWrapper->getPublicKey(clientRegisterRequest.data.payload.publicKey, RSAPublicWrapper::KEYSIZE);

    //TODO Init with m.data.name = { 0 }
    initMessage(clientRegisterRequest.data.payload.name, 255);
    memcpy(clientRegisterRequest.data.payload.name, userName.c_str(), userName.length());

    memcpy(clientRegisterRequseBuffer, clientRegisterRequest.buffer, sizeof(ClientRegisterRequest));
}

std::string UserManager::saveMyInfoFile(const std::string& userName, const std::string& clientId, const std::string& base64PrivateKey)
{
    std::fstream myInfoFile;
    std::string saveFileResultStr;

    myInfoFile.open(UserManager::FILE_NAME, std::ios::out);
    if (myInfoFile.is_open())
    {
        myInfoFile << userName << std::endl;
        myInfoFile << clientId << std::endl;
        myInfoFile << base64PrivateKey << std::endl;
        myInfoFile.close();

        saveFileResultStr += "Save new user properties to " + FILE_NAME + " file success!\n";
    }
    else
    {
        //TODO Handle this issue
        saveFileResultStr += "Can't access to the file " + FILE_NAME + ", save user properties failed!\n";        
    }

    return saveFileResultStr;
}

bool UserManager::isMyInfoFileExists()
{
    std::fstream myInfoFile;
    myInfoFile.open(FILE_NAME, std::ios::in);
    if (myInfoFile.is_open())
    {        
        myInfoFile.close();
        return true;
    }
    else
    {
        return false;
    }
}

void UserManager::initMessage(char* message, int length)
{
    for (int i = 0; i < length; i++)
        message[i] = '\0';
}

#pragma endregion
