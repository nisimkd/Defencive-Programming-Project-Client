#pragma once
#include <boost/uuid/uuid.hpp>
#include <string>
#include "RSAWrapper.h"
#include "TcpClient.h"

class UserManager
{
private:
	#pragma region Constants

	const std::string FILE_NAME = "my.info";

	#define VERSION 1;
	#define USER_NAME_SIZE 255

	#pragma endregion

	#pragma region Messages structs

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

	#pragma endregion

	#pragma region Enums

	enum requestCode
	{
		user_register = 110,
		request_clients_list = 120,
		request_public_key = 130,
		request_waiting_messages = 140,
		send_text_message = 150,
		request_symmetric_key = 151,
		send_symmetric_key = 152,
	};

	#pragma endregion

	#pragma region Members

	TcpClient* tcpClient;
	std::string userName;
	boost::uuids::uuid clientId;
	RSAPrivateWrapper* rsaPrivateWrapper;
	uint8_t version;

	#pragma endregion

	#pragma region Methods

	void createClientUserRequestBuffer(char*);
	bool isMyInfoFileExists();
	std::string saveMyInfoFile(const std::string&, const std::string&, const std::string&);
	void initMessage(char* message, int length);

	#pragma endregion

public:
	#pragma region Constructors and Destructors

	UserManager(const std::string&, const std::string&);
	~UserManager();

	#pragma endregion	

	#pragma region Methods

	std::string registerUser(const std::string&);

	#pragma endregion

};

