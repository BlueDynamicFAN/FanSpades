//	MessageProtocol.h
//
//	Purpouse: Describes the ruls server and client side comunicate 
//  author: Veronika Kotckovich

#ifndef MessageProtocol_HG_
#define MessageProtocol_HG_

#include <string>
#include "Buffer.h"
#include "cCard.h"

struct Body{
	std::string roomName;
	std::string message;
	std::string name;
};

struct Header {
	size_t packet_length;
	short command_id ;
};

class MessageProtocol {
public:
	MessageProtocol();
	~MessageProtocol();

	void readHeader(Buffer &myBuffer);
	void sendMessage(Buffer &myBuffer);
	void sendMessage(Buffer &myBuffer, int id);

	void receiveName(Buffer &myBuffer);

	void createBuffer(size_t index);
	void sendNewVel(Buffer &myBuffer, int id, std::string x, std::string y, std::string z);
	void sendDeck(Buffer &myBuffer, std::vector<cCard*> deck);
	void receiveID(Buffer &myBuffer, int &id);
	void removeCardFromTable(Buffer & myBuffer, int id, int &cardId);
	void receiveCard(Buffer &myBuffer, std::vector<cCard*> &deck, std::vector<cCard*> &cards);
	void sendCardId(Buffer &myBuffer, int &id, int &cardId);
	Header messageHeader;
	Body messageBody;
	Buffer* buffer;
};
#endif // !MessageProtocol_HG_

//Protocol Example :
//Header
//int int
//[packet_length][message_id]
//Send message
//? int string int string
//[header][length]][room_name][length][message]
//Receive message
//? int string int string int string
//[header][length][name][length][message][length][room_name]
//Join Room :
//? int string
//[header][length][room_name]
//Leave Room :
//? int string
//[header][length][room_name]