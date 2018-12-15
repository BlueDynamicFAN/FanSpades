//	MessageProtocol.h
//
//	Purpouse: Describes the ruls server and client side comunicate 
//  author: Veronika Kotckovich

#ifndef MessageProtocol_HG_
#define MessageProtocol_HG_

#include <string>
#include "Buffer.h"

struct Body{
	std::string message;
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
	void sendMessage(Buffer &myBuffer, int id);
	void sendMessage(Buffer &myBuffer);
	void receiveMessage(Buffer &myBuffer);
	void sendID(Buffer &myBuffer, int cardID, int commandID);
	void createBuffer(size_t index);
	void receiveDeck(Buffer &myBuffer, std::vector<int> &playerDeck, std::vector<int> &otherDeck);
	void sendCard(Buffer &myBuffer, int cardId, int posId);
	void receiveNewVelocity(Buffer &myBuffer, int &id, float &x, float &y, float &z);
	void receiveCardId(Buffer &myBuffer, int &cardId);
	
	Header messageHeader;
	Body messageBody;
	Buffer* buffer;
};
#endif // !MessageProtocol_HG_