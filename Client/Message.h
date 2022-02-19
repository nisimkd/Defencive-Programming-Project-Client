#pragma once
#include <boost/uuid/uuid.hpp>
#include <string>
class Message
{
private:
	boost::uuids::uuid clientId;	
	uint8_t messageType;
	std::string content;
public:
	Message(boost::uuids::uuid, uint8_t, const std::string&);
	~Message();
	uint8_t getMessageType();
	boost::uuids::uuid getClientId();
	std::string getMessageContent();
};

