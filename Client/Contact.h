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
	unsigned char symmetricKey[AESWrapper::DEFAULT_KEYLENGTH];		
	bool hasSymmetricKey = false;
	bool hasPublicKey = false;
	Contact(char name[USER_NAME_SIZE], boost::uuids::uuid id)
	{
		memcpy(userName, name, USER_NAME_SIZE);
		clientId = id;		
	}
	~Contact()
	{

	}

	void setSymmetricKey(unsigned char* key)
	{
		memcpy(symmetricKey, key, AESWrapper::DEFAULT_KEYLENGTH);
		hasSymmetricKey = true;
	}

	bool getHasSymmetricKey()
	{
		return hasSymmetricKey;
	}

	unsigned char* getSymmetricKey()
	{
		return symmetricKey;
	}
};

