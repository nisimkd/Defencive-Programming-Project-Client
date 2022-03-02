#pragma once
#include "AESWrapper.h"
#include <boost/uuid/uuid.hpp>
#include "RSAWrapper.h"

const unsigned short USER_NAME_SIZE = 255;
struct Contact
{	
public:
	boost::uuids::uuid clientId;
	char userName[USER_NAME_SIZE];
	char publicKey[RSAPublicWrapper::KEYSIZE];
	AESWrapper *aes;
	bool hasSymmetricKey = false;
	bool hasPublicKey = false;
	Contact(char name[USER_NAME_SIZE], boost::uuids::uuid id);
	~Contact();
	bool getHasSymmetricKey();
};

