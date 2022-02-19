#pragma once
#include <boost/uuid/uuid.hpp>
#include <string>
#include "RSAWrapper.h"
#include "TcpClient.h"
#include "Contact.h"
#include "Message.h"

class UserManager
{
private:
	#pragma region Constants

	const std::string INFO_FILE_NAME = "my.info";

	#define VERSION 1;
	#define USER_NAME_SIZE 255
	#define EMPTY_PAYLOAD_SIZE 0

	#pragma endregion


	#pragma region Client request structs


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
	struct ClientRegisterUserRequestPayload
	{
		char name[USER_NAME_SIZE];
		char publicKey[RSAPublicWrapper::KEYSIZE];
	};
	#pragma pack(pop)
	#pragma pack(push, 1)
	struct clientRegisterUserRequestData
	{
		ClientRequestHeader header;
		ClientRegisterUserRequestPayload payload;
	};
	#pragma pack(pop)
	union ClientRegisterRequest
	{
		clientRegisterUserRequestData data;
		char buffer[sizeof(clientRegisterUserRequestData)];
	};


	#pragma pack(push, 1)
	struct requestNoPayloadData
	{
		ClientRequestHeader header;		
	};
	#pragma pack(pop)
	union RequestNoPayload
	{
		requestNoPayloadData data;
		char buffer[sizeof(requestNoPayloadData)];
	};


	#pragma pack(push, 1)
	struct requestPublicKeyRequestData
	{
		ClientRequestHeader header;
		boost::uuids::uuid requestedClientId;
	};
	#pragma pack(pop)
	union RequestPublicKeyRequest
	{
		requestPublicKeyRequestData data;
		char buffer[sizeof(requestPublicKeyRequestData)];
	};

	#pragma endregion


	#pragma region Server responses structs

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

	#pragma pack(push, 1)
	struct ServerContactData
	{
		boost::uuids::uuid clientId;
		char name[USER_NAME_SIZE];
	};	
	#pragma pack(pop)

	#pragma pack(push, 1)
	struct PublicKeyRequestResponse
	{
		ServerResponseHeader serverResponseHeader;
		boost::uuids::uuid clientId;
		char publicKey[RSAPublicWrapper::KEYSIZE];
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	//Todo Improve the logic with this struct
	struct MessageResponseHeader
	{
		boost::uuids::uuid clientId;
		uint32_t messageId;
		uint8_t messageType;
		uint32_t messageSize;
	};
	#pragma pack(pop)

	#pragma endregion

	#pragma region Enums

	enum requestCodeType
	{
		user_register = 1100,
		request_clients_list = 1101,
		request_public_key = 1102,		
		send_text_message = 1103,
		request_waiting_messages = 1104,
	};

	enum serverResponseCodeType
	{
		register_user_success = 2100,
		get_users_list = 2101,
		get_public_key = 2102,
		message_to_user_sent = 2103,
		get_waiting_messages = 2104,
		server_error = 9000,
	};

	enum class messageType
	{
		request_symmetric_key = 1,
		send_symmetric_key = 2,
		send_text_message = 3
	};

	#pragma endregion

	#pragma region Members

	TcpClient* tcpClient;
	std::string userName;
	boost::uuids::uuid clientId;
	RSAPrivateWrapper* rsaPrivateWrapper;
	std::map<boost::uuids::uuid, Contact> contacts;

	#pragma endregion

	#pragma region Methods

	void createRegisterUserRequestBuffer(char*);
	void createNoPayloadRequestBuffer(char*, requestCodeType);
	void createPublicKeyRequestBuffer(char*, boost::uuids::uuid);
	bool isMyInfoFileExists();
	std::string saveMyInfoFile(const std::string&, const std::string&, const std::string&);
	void initMessage(char*, int);
	std::string addContactsFromServerToList(char*, uint32_t);
	std::string createWaitingMessagesStr(char*, uint32_t);
	std::string handleMessage(Message);
	bool getClientIdByUserName(const std::string&, boost::uuids::uuid&);

	#pragma endregion

public:
	#pragma region Constructors and Destructors

	UserManager(const std::string&, const std::string&);
	~UserManager();

	#pragma endregion	

	#pragma region Methods

	std::string registerUser(const std::string&);
	std::string requestClientsListFromServer();
	std::string requestPublicKey(const std::string&);
	std::string requestWaitingMessages();

	#pragma endregion

};

