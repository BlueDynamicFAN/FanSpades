//	MessageProtocol.cpp
//
//	Purpouse: Describes the ruls server and client side comunicate 
//  author: Veronika Kotckovich

#include "MessageProtocol.h"
#include <iostream>

//Constructor
//
//Puspouse: Creates MessageProtocol
MessageProtocol::MessageProtocol()
{
	this->buffer = NULL;
}

//Destructor 
//
//Puspouse: deletes buffer
MessageProtocol::~MessageProtocol()
{
	delete this->buffer;
}

//createBuffer(size)
//
//Puspouse: Creates a budder of provided size
//
//@param: size_t size
//@return: void
void MessageProtocol::createBuffer(size_t size)
{
	this->buffer = new Buffer(size);
}


//readHeader(&myBuffer)
//
//Purpouse: Protocol rule for the messages' header 
//
//int int
//[packet_length][message_id]
//
//@param: Buffer &myBuffer -  the reference to current buffer
void MessageProtocol::readHeader(Buffer &myBuffer)
{
	this->messageHeader.packet_length = myBuffer.ReadInt32LE();
	this->messageHeader.command_id = myBuffer.ReadShort16LE();
	return;
}

//receiveMessage()
//
//Purpouse: Receiving a message
//
//? int string int string int string
//[header][length][message]
//
//@param: Buffer &myBuffer -  the reference to current buffer

void MessageProtocol::receiveMessage(Buffer &myBuffer)
{
	//readHeader(myBuffer);
	int length = myBuffer.ReadInt32LE();
	for (int i = 0; i <= length - 1; i++)
	{
		this->messageBody.message += myBuffer.ReadChar8LE();
	}
}

void MessageProtocol::receiveDeck(Buffer &myBuffer, std::vector<int> &playerDeck, std::vector<int> &otherDeck)
{
	//readHeader(myBuffer);
	int size = myBuffer.ReadInt32LE();
	std::cout << "SIZE " << size << std::endl;
	for (int i = 0; i < 10; i++)
	{
		int newCard = myBuffer.ReadInt32LE();
		playerDeck.push_back(newCard);
	}
	for (int i = 10; i < size; i++)
	{
		int newCard = myBuffer.ReadInt32LE();
		otherDeck.push_back(newCard);
	}
}

void MessageProtocol::sendID(Buffer &myBuffer, int cardID, int commandID)
{
	this->messageHeader.command_id = commandID;
	this->messageHeader.packet_length = sizeof(int) + sizeof(int) + sizeof(short);
	myBuffer.resizeBuffer(this->messageHeader.packet_length);
	myBuffer.WriteInt32LE(this->messageHeader.packet_length);
	myBuffer.WriteShort16LE(this->messageHeader.command_id);
	myBuffer.WriteInt32LE(cardID);
}

void MessageProtocol::receiveNewVelocity(Buffer &myBuffer, int &id, float &x, float &y, float &z)
{
	id = myBuffer.ReadInt32LE();

	int sizeX = myBuffer.ReadInt32LE();
	std::string xS;
	for (int i = 0; i != sizeX; i++)
	{
		xS += myBuffer.ReadChar8LE();
	}

	int sizeY = myBuffer.ReadInt32LE();
	std::string yS;
	for (int i = 0; i != sizeX; i++)
	{
		yS += myBuffer.ReadChar8LE();
	}

	int sizeZ = myBuffer.ReadInt32LE();
	std::string zS;
	for (int i = 0; i != sizeX; i++)
	{
		zS += myBuffer.ReadChar8LE();
	}

	x = std::stof(xS);
	y = std::stof(yS);
	z = std::stof(zS);
}


//Send message -- command id = 01
//
//Purpose: Sending message to the server with custom command id
//
//? int string int string
//[header][length][message]
//
//@param: void
//@return: void
void MessageProtocol::sendMessage(Buffer &myBuffer, int id)
{
	this->messageHeader.command_id = id;
	this->messageHeader.packet_length = sizeof(int) + sizeof(short) + sizeof(int) + this->messageBody.message.length();

	myBuffer.resizeBuffer(this->messageHeader.packet_length);
	myBuffer.WriteInt32LE(this->messageHeader.packet_length);
	myBuffer.WriteShort16LE(this->messageHeader.command_id);
	myBuffer.WriteInt32LE(this->messageBody.message.length());
	const  char *temp = this->messageBody.message.c_str();
	for (int i = 0; temp[i] != '\0'; i++)
	{
		myBuffer.WriteChar8LE(temp[i]);
	}

}


//Send message -- command id = 01
//
//Purpose: Sending message to the server
//
//? int string int string
//[header][length][name][length][message]
//
//@param: void
//@return: void
void MessageProtocol::sendMessage(Buffer &myBuffer)
{
	this->messageHeader.command_id = 01;
	this->messageHeader.packet_length = sizeof(int) + sizeof(short) + sizeof(int) +
		sizeof(int) + this->messageBody.message.length();

	myBuffer.resizeBuffer(this->messageHeader.packet_length);
	myBuffer.WriteInt32LE(this->messageHeader.packet_length);
	myBuffer.WriteShort16LE(this->messageHeader.command_id);
	myBuffer.WriteInt32LE(this->messageBody.message.length());
	const  char *temp = this->messageBody.message.c_str();
	for (int i = 0; temp[i] != '\0'; i++)
	{
		myBuffer.WriteChar8LE(temp[i]);
	}

}

void MessageProtocol::sendCard(Buffer &myBuffer, int cardId, int posId)
{
	this->messageHeader.command_id = 04;
	this->messageHeader.packet_length = sizeof(int) + sizeof(short) + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(int);

	myBuffer.resizeBuffer(this->messageHeader.packet_length);
	myBuffer.WriteInt32LE(this->messageHeader.packet_length);
	myBuffer.WriteShort16LE(this->messageHeader.command_id);

	myBuffer.WriteInt32LE(sizeof(cardId));
	myBuffer.WriteInt32LE(cardId);

	myBuffer.WriteInt32LE(posId);
}

void MessageProtocol::receiveCardId(Buffer &myBuffer, int &cardId)
{
	int id = myBuffer.ReadInt32LE();
	std::cout << id << std::endl;
	cardId = id;
}