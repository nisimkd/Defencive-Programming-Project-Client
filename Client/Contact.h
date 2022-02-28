#pragma once
#include "AESWrapper.h"
#include <boost/uuid/uuid.hpp>
#include "RSAWrapper.h"

//TODO Change to Contact class and improve memory issues
const unsigned short USER_NAME_SIZE = 255;
struct Contact
{	
	boost::uuids::uuid clientId;
	char userName[USER_NAME_SIZE];
	char publicKey[RSAPublicWrapper::KEYSIZE];
	AESWrapper *aes;
	bool hasSymmetricKey = false;
	bool hasPublicKey = false;
	Contact(char name[USER_NAME_SIZE], boost::uuids::uuid id)
	{
		memcpy(userName, name, USER_NAME_SIZE);
		clientId = id;		
	}
	~Contact()
	{
		delete aes;
	}

	bool getHasSymmetricKey()
	{
		return hasSymmetricKey;
	}
};

