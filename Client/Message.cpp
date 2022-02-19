#include "Message.h"
Message::Message(boost::uuids::uuid clientId, uint8_t messageType, const std::string& content)
{
	this->clientId = clientId;	
	this->messageType = messageType;	
	this->content = content;
}
Message::~Message()
{

}

uint8_t Message::getMessageType()
{
	return messageType;
}
boost::uuids::uuid Message::getClientId()
{
	return clientId;
}
std::string Message::getMessageContent()
{
	return content;
}
