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

//createBuffer()
//
//Puspouse: Creates a buffer
//
//@param: size_t size
//@return: void
void MessageProtocol::createBuffer(size_t size)
{
	this->buffer = new Buffer(size);
}


//readHeader()
//
//Purpouse: Reads the message's header 
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




void MessageProtocol::receiveID(Buffer &myBuffer, int &id)
{
	id = myBuffer.ReadInt32LE();
}

void MessageProtocol::sendDeck(Buffer &myBuffer, std::vector<cCard*> deck)
{
	this->messageHeader.command_id = 01;
	this->messageHeader.packet_length = sizeof(int) + sizeof(short) + sizeof(int) + sizeof(int) * 21;
	myBuffer.resizeBuffer(this->messageHeader.packet_length);
	myBuffer.WriteInt32LE(this->messageHeader.packet_length);
	myBuffer.WriteShort16LE(this->messageHeader.command_id);
	myBuffer.WriteInt32LE(deck.size());

	for (int i = 0; i != deck.size(); i++)
	{
		myBuffer.WriteInt32LE(deck[i]->id);
	}
}

void MessageProtocol::sendNewVel(Buffer &myBuffer, int id, std::string x, std::string y, std::string z)
{
	this->messageHeader.command_id = 04;
	this->messageHeader.packet_length = sizeof(int) + sizeof(short) + x.length() + y.length() + z.length() + sizeof(int) * 3;
	myBuffer.resizeBuffer(this->messageHeader.packet_length);
	myBuffer.WriteInt32LE(this->messageHeader.packet_length);
	myBuffer.WriteShort16LE(this->messageHeader.command_id);

	myBuffer.WriteInt32LE(id);
	myBuffer.WriteInt32LE(x.length());
	const  char *tempX = x.c_str();
	for (int i = 0; tempX[i] != '\0'; i++)
	{
		myBuffer.WriteChar8LE(tempX[i]);
	}

	myBuffer.WriteInt32LE(y.length());
	const  char *tempY = y.c_str();
	for (int i = 0; tempY[i] != '\0'; i++)
	{
		myBuffer.WriteChar8LE(tempY[i]);
	}

	myBuffer.WriteInt32LE(z.length());
	const  char *tempZ = z.c_str();
	for (int i = 0; tempZ[i] != '\0'; i++)
	{
		myBuffer.WriteChar8LE(tempZ[i]);
	}
}


//receiveMessage()
//
//Purpouse: Receiving a message
//
//? int string int string int string
//[header][length][message]
//
//@param: Buffer &myBuffer -  the reference to current buffer

//void MessageProtocol::receiveMessage(Buffer &myBuffer)
//{
//	int mameLength = myBuffer.ReadInt32LE();
//	//printf("receiving package len %i ", this->messageHeader.packet_length);
//	for (int i = 0; i <= mameLength - 1; i++)
//	{
//		this->messageBody.name += myBuffer.ReadChar8LE();
//	}
//
//	int length = myBuffer.ReadInt32LE();
//	//printf("receiving package len %i ", this->messageHeader.packet_length);
//	for (int i = 0; i <= length - 1; i++)
//	{
//		this->messageBody.message += myBuffer.ReadChar8LE();
//	}
//}



//receiveName()
//
//Purpouse: Receives a client's name
//
//? int string 
//[header][length][name]
//
//@param: Buffer &myBuffer -  the reference to current buffer
void MessageProtocol::receiveName(Buffer &myBuffer)
{
	int length = myBuffer.ReadInt32LE();
	//printf("receiving package len %i ", this->messageHeader.packet_length);
	for (int i = 0; i <= length - 1; i++)
	{
		this->messageBody.message += myBuffer.ReadChar8LE();
	}
}

//Send message -- command id = 01
//? int string int string
//[header][length][message]
void MessageProtocol::sendMessage(Buffer &myBuffer)
{
	this->messageHeader.command_id = 01;
	this->messageHeader.packet_length = sizeof(int) + sizeof(short) + sizeof(int) + this->messageBody.message.length();
	//printf("Sending packet length %i ", this->messageHeader.packet_length);

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

//Send message -- with custome command id
//? int string int string
//[header][length][message]
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


void MessageProtocol::receiveCard(Buffer &myBuffer, std::vector<cCard*> &deck, std::vector<cCard*> &cards)
{
	int size = myBuffer.ReadInt32LE();
	int cardId = myBuffer.ReadInt32LE();
	int posId = myBuffer.ReadInt32LE();

	std::cout << "Card played is " << deck[posId]->info << std::endl;

	if (deck[posId]->id == cardId)
	{
		cards.push_back(deck[posId]);
	}
}

void MessageProtocol::sendCardId(Buffer & myBuffer, int &id, int &cardId)
{	
	this->messageHeader.command_id = id;
	this->messageHeader.packet_length = sizeof(int) + sizeof(int) + sizeof(short);
	myBuffer.resizeBuffer(this->messageHeader.packet_length);
	myBuffer.WriteInt32LE(this->messageHeader.packet_length);
	myBuffer.WriteShort16LE(this->messageHeader.command_id);
	myBuffer.WriteInt32LE(cardId);
}
