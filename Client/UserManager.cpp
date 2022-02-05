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
    this->contacts = std::map<boost::uuids::uuid, Contact>();
}

UserManager::~UserManager()
{
    delete rsaPrivateWrapper;
    delete tcpClient;
}

#pragma endregion

#pragma region Public Methods

std::string UserManager::registerUser(const std::string& userName)
{    
    if (UserManager::isMyInfoFileExists())
    {
        return "File " + INFO_FILE_NAME + " already exists, can't register new user!\n";
    }

    std::string registerUserResultStr;

    // Create client request header and payload

    char requestBuffer[BUFFER_SIZE];    
    createRegisterUserRequestBuffer(requestBuffer);

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

        if (serverRegisterUserResponse.serverResponseHeader.code == serverResponseCodeType::server_error)
        {
            registerUserResultStr = "Server responded with an error, failed to register new user, please try again\n";
        }
        else if (serverRegisterUserResponse.serverResponseHeader.code == serverResponseCodeType::register_user_success)
        {
            std::string responseLogMessage = "Response data: Version: " + std::to_string((int)serverRegisterUserResponse.serverResponseHeader.version) + ", Code: " +
                std::to_string(serverRegisterUserResponse.serverResponseHeader.code) + ", Payload Size: " + std::to_string(serverRegisterUserResponse.serverResponseHeader.payloadSize)
                + ", ClientId: " + to_string(serverRegisterUserResponse.clientId) + "\n";
            Logger::info(responseLogMessage);

            registerUserResultStr = "Register new user success!\n";

            std::string base64PrivateKeyStr = Base64Wrapper::encode(rsaPrivateWrapper->getPrivateKey());
            registerUserResultStr += saveMyInfoFile(userName, to_string(serverRegisterUserResponse.clientId), base64PrivateKeyStr);
            this->userName = userName;
            this->clientId = serverRegisterUserResponse.clientId;
        }
        else
        {
            registerUserResultStr = "Unknown server error, failed to register new user, please try again\n";
        }

        delete response;
    }

    return registerUserResultStr;
}

std::string UserManager::requestClientsListFromServer()
{
    std::string getClientsListStr;

    // Create request buffer
    char requestBuffer[BUFFER_SIZE];
    createClientsListRequestBuffer(requestBuffer);
    
    char* response = tcpClient->sendRequestToServer(requestBuffer);
    if (response == NULL)
    {
        getClientsListStr = "Failed access to the server, failed to get clients list, please try again later!\n";
    }
    else
    {        
        ServerResponseHeader responseHeader;
        memcpy(&responseHeader, response, sizeof(ServerResponseHeader));

        if (responseHeader.code == serverResponseCodeType::server_error)
        {
            getClientsListStr = "Server responded with an error, failed to get clients list, please try again\n";
        }
        else if (responseHeader.code == serverResponseCodeType::get_users_list)
        {
            getClientsListStr = addContactsFromServerToList(response, responseHeader.payloadSize);
        }
        else
        {
            getClientsListStr = "Unknown server error, failed to get clients list, please try again\n";
        }

        delete response;
    }

    return getClientsListStr;
}

std::string UserManager::requestPublicKey(const std::string& userName)
{    
    boost::uuids::uuid requestedClientId;

    if (!getClientIdByUserName(userName, requestedClientId))
        return "User with user name " + userName + " not found";

    std::string requestPublicKeyResult;

    // Create request buffer
    char requestBuffer[BUFFER_SIZE];
    createPublicKeyRequestBuffer(requestBuffer, requestedClientId);

    char* response = tcpClient->sendRequestToServer(requestBuffer);
    if (response == NULL)
    {
        requestPublicKeyResult = "Failed access to the server, failed to get public key of user " + userName + ", please try again later!\n";
    }
    else
    {
        PublicKeyRequestResponse publicKeyRequestResponse;
        memcpy(&publicKeyRequestResponse, response, sizeof(PublicKeyRequestResponse));

        if (publicKeyRequestResponse.serverResponseHeader.code == serverResponseCodeType::server_error)
        {
            requestPublicKeyResult = "Server responded with an error, failed to get public key of user " + userName + ", please try again later!\n";
        }
        else if (publicKeyRequestResponse.serverResponseHeader.code == serverResponseCodeType::get_public_key)
        {
            //TODO Handle client Id error
            auto requestedContact = contacts.find(publicKeyRequestResponse.clientId);
            memcpy(requestedContact->second.publicKey, publicKeyRequestResponse.publicKey, RSAPublicWrapper::KEYSIZE);
            requestPublicKeyResult = "Get public key of user " + userName + " success\n";
        }
        else
        {
            requestPublicKeyResult = "Unknown server error, failed to get public key of user " + userName + ", please try again later!\n";
        }

        delete response;
    }

    return requestPublicKeyResult;
}

#pragma endregion

#pragma region Private Methods

void UserManager::createRegisterUserRequestBuffer(char *clientRegisterRequseBuffer)
{
    ClientRegisterRequest clientRegisterRequest;
    clientRegisterRequest.data.header.version = VERSION;
    clientRegisterRequest.data.header.code = UserManager::requestCodeType::user_register;
    clientRegisterRequest.data.header.payloadSize = sizeof(ClientRegisterUserRequestPayload);
    rsaPrivateWrapper->getPublicKey(clientRegisterRequest.data.payload.publicKey, RSAPublicWrapper::KEYSIZE);

    //TODO Init with m.data.name = { 0 }
    initMessage(clientRegisterRequest.data.payload.name, 255);
    memcpy(clientRegisterRequest.data.payload.name, userName.c_str(), userName.length());

    memcpy(clientRegisterRequseBuffer, clientRegisterRequest.buffer, sizeof(ClientRegisterRequest));
}

void UserManager::createClientsListRequestBuffer(char* clientsListRequestBuffer)
{
    ClientsListRequest clientsListRequest;
    clientsListRequest.data.header.version = VERSION;
    clientsListRequest.data.header.code = UserManager::requestCodeType::request_clients_list;
    clientsListRequest.data.header.payloadSize = EMPTY_PAYLOAD_SIZE;

    memcpy(clientsListRequestBuffer, clientsListRequest.buffer, sizeof(ClientsListRequest));
}

void UserManager::createPublicKeyRequestBuffer(char* publicKeyRequestBuffer, boost::uuids::uuid requestedClientId)
{
    RequestPublicKeyRequest requestPublicKeyRequest;
    requestPublicKeyRequest.data.header.clientId = clientId;
    requestPublicKeyRequest.data.header.code = requestCodeType::request_public_key;
    requestPublicKeyRequest.data.header.version = VERSION;
    requestPublicKeyRequest.data.header.payloadSize = sizeof(boost::uuids::uuid);
    requestPublicKeyRequest.data.requestedClientId = requestedClientId;

    memcpy(publicKeyRequestBuffer, requestPublicKeyRequest.buffer, sizeof(RequestPublicKeyRequest));
}

std::string UserManager::saveMyInfoFile(const std::string& userName, const std::string& clientId, const std::string& base64PrivateKey)
{
    std::fstream myInfoFile;
    std::string saveFileResultStr;

    myInfoFile.open(UserManager::INFO_FILE_NAME, std::ios::out);
    if (myInfoFile.is_open())
    {
        myInfoFile << userName << std::endl;
        myInfoFile << clientId << std::endl;
        myInfoFile << base64PrivateKey << std::endl;
        myInfoFile.close();

        saveFileResultStr += "Save new user properties to " + INFO_FILE_NAME + " file success!\n";
    }
    else
    {
        //TODO Handle this issue
        saveFileResultStr += "Can't access to the file " + INFO_FILE_NAME + ", save user properties failed!\n";
    }

    return saveFileResultStr;
}

bool UserManager::isMyInfoFileExists()
{
    std::fstream myInfoFile;
    myInfoFile.open(INFO_FILE_NAME, std::ios::in);
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

std::string UserManager::addContactsFromServerToList(char* responseBuffer, uint32_t payloadSize)
{
    size_t serverContactsCount = payloadSize / sizeof(ServerContactData);

    //TODO Using better allocation
    ServerContactData *serverContactsData = new ServerContactData[serverContactsCount];
    memcpy(serverContactsData, responseBuffer + sizeof(ServerResponseHeader), payloadSize);

    std::string contactsStr = "Contacts list:\n";

    for (int i = 0; i < serverContactsCount; i++)
    {
        Contact currentContact = Contact(serverContactsData[i].name, serverContactsData[i].clientId);
        contacts.insert(std::pair<boost::uuids::uuid, Contact>(currentContact.clientId, currentContact));
        contactsStr += std::string(currentContact.userName);
    }

    delete[] serverContactsData;
    return contactsStr;
}

bool UserManager::getClientIdByUserName(const std::string& userName, boost::uuids::uuid &clientId)
{
    for (auto const& contact : contacts)
    {
        if (std::string(contact.second.userName).compare(userName) == 0)
        {
            clientId = contact.second.clientId;
            return true;
        }        
    }

    return false;
}

#pragma endregion