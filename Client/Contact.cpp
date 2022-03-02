#include "Contact.h"

Contact::Contact(char name[USER_NAME_SIZE], boost::uuids::uuid id)
{
	memcpy(userName, name, USER_NAME_SIZE);
	clientId = id;
}
Contact::~Contact()
{
	delete aes;
}

bool Contact::getHasSymmetricKey()
{
	return hasSymmetricKey;
}
