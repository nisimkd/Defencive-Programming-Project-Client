#pragma once
#include "AESWrapper.h"
#include <boost/uuid/uuid.hpp>
#include "RSAWrapper.h"

const unsigned short USER_NAME_SIZE = 255;
struct Contact
{	
	boost::uuids::uuid clientId;
	char userName[USER_NAME_SIZE];
	char publicKey[RSAPublicWrapper::KEYSIZE];
	unsigned char symmetricKey[AESWrapper::DEFAULT_KEYLENGTH];	
	Contact(char name[USER_NAME_SIZE], boost::uuids::uuid id)
	{
		memcpy(userName, name, USER_NAME_SIZE);
		clientId = id;		
	}
};

