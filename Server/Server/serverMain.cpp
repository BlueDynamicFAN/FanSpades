//Server
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include "MessageProtocol.h"
#include <vector>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "cCard.h"

std::vector<cCard*> createDeck();
std::vector<cCard*> cardsPlayed;
int checkRound(std::vector<cCard*> &cardsPlayed, int &dealerId, int &otherId);

int dealerId = 0;
int otherId = 0;
int roundNum = 0;

std::vector<Position> positions = { Position{35.0f, 0.0f, 67.0f}, Position{35.0f, 0.0f, 52.0f}, Position{35.0f, 0.0f, 37.0f}, Position{35.0f, 0.0f, 22.0f}, Position{35.0f, 0.0f, 7.0f}, Position{35.0f, 0.0f, -8.0f}, Position{35.0f, 0.0f, -23.0f}, Position{35.0f, 0.0f, -38.0f}, Position{35.0f, 0.0f, -53.0f}, Position{35.0f, 0.0f, -68.0f} };

struct client
{
	SOCKET Connection;
	std::string name = "";
	std::string room = "";
	bool isPlaying = false;
	int GameId = -1;
	std::vector<cCard*> playerDeck;
	int score = 0;
};

struct Game
{
	std::vector<cCard*> theCardDeck = createDeck();
	int numCardsPerPlayer = 10;
	int player1id, player2id;
	float lastY = 0.0f;
};

client Clients[100];
std::vector<int> lobby;
std::vector<Game*> Games;

int clientsCounter = 0;
void sendDeckToClient(int plyerID, std::vector<cCard*> theDeck);
void sendMessageToClient(SOCKET theConnection, int id, std::string message);
void sendCardIdToOther(int clientIndex, int id, int cardId);

void handleClients(int index)
{
	int packLength;
	std::string name = "";
	bool run = true;
	srand(time(NULL));
	while (run)
	{

		if (!Clients[index].isPlaying)
		{
			if (lobby.size() >= 2)
			{
				Game* newGame = new Game();
				int anotherPlayer = -1;
				if (lobby.size() == 2)
				{
					newGame->player1id = lobby[0];
					newGame->player2id = lobby[1];
					lobby.erase(lobby.begin(), lobby.begin() + 2);

					std::cout << "\nplayer1id " + std::to_string(newGame->player1id) + "\nplayer2id " + std::to_string(newGame->player2id) << std::endl;
				}


				//Distribute the cards
				bool done = false;
				while (!done)
				{
					if (Clients[newGame->player1id].playerDeck.size() < newGame->numCardsPerPlayer)
					{
						int cardIndex = rand() % newGame->theCardDeck.size();
						//std::cout << "CARDS SIZE " << newGame->theCardDeck.size() << " cardIndex " << cardIndex << std::endl;
						Clients[newGame->player1id].playerDeck.push_back(newGame->theCardDeck[cardIndex]);
						newGame->theCardDeck.erase(newGame->theCardDeck.begin() + cardIndex);
					}
					else {
						Clients[newGame->player2id].playerDeck = newGame->theCardDeck;
						done = true;
					}
				}

				Games.push_back(newGame);
				Clients[newGame->player1id].GameId = Games.size() - 1;
				Clients[newGame->player2id].GameId = Games.size() - 1;
				Clients[newGame->player1id].isPlaying = true;
				Clients[newGame->player2id].isPlaying = true;

				sendMessageToClient(Clients[newGame->player1id].Connection, 0, "Starting the game");
				sendMessageToClient(Clients[newGame->player2id].Connection, 0, "Starting the game");

				for (int i = 0; i < 10; i++)
				{
					Clients[newGame->player1id].playerDeck[i]->position = positions[i];
					Clients[newGame->player1id].playerDeck[i]->playerId = newGame->player1id;
					Clients[newGame->player2id].playerDeck[i]->position = positions[i];
					Clients[newGame->player2id].playerDeck[i]->playerId = newGame->player2id;
				}

				dealerId = newGame->player1id;
				otherId = newGame->player2id;

				sendDeckToClient(newGame->player1id, Clients[newGame->player1id].playerDeck);
				sendDeckToClient(newGame->player2id, Clients[newGame->player2id].playerDeck);

			}

		}
		else {
			std::vector<char> packet(512);
			if ((packLength = recv(Clients[index].Connection, &packet[0], packet.size(), NULL)) < 1) {
				closesocket(Clients[index].Connection);
				//WSACleanup();
				if (!Clients[index].isPlaying)
				{
					std::vector<int>::iterator it = std::find(lobby.begin(), lobby.end(), index);
					if (it != lobby.end())
					{
						int arrayPosition = std::distance(lobby.begin(), it);
						lobby.erase(lobby.begin() + arrayPosition);
						std::cout << "Lobby size: " << lobby.size() << std::endl;;
					}
				}
				run = false;
			}
			else
			{
				MessageProtocol* messageProtocol = new MessageProtocol();
				messageProtocol->createBuffer(512);

				messageProtocol->buffer->mBuffer = packet;
				messageProtocol->readHeader(*messageProtocol->buffer);

				if (messageProtocol->messageHeader.command_id == 1)
				{
					//std::cout << "Clients asks for updates" << lobby.size() << std::endl;;
				}

				if (messageProtocol->messageHeader.command_id == 03)
				{
					int id;
					messageProtocol->receiveID(*messageProtocol->buffer, id);
					std::cout << id << std::endl;

					float x, y, z;

					x = 5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (25 - 5)));
					x *= (-1);
					z = ((float(rand()) / float(RAND_MAX)) * (35.0 - (-35.0))) + (-35.0);

					int gameId = Clients[index].GameId;
					float lastY = Games[gameId]->lastY;
					lastY += 0.03;
					y = lastY;
					Games[gameId]->lastY = lastY;

					std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;


					std::string sX = std::to_string(x);
					std::string sY = std::to_string(y);
					std::string sZ = std::to_string(z);

					MessageProtocol* messageSendProtocol = new MessageProtocol();
					messageSendProtocol->createBuffer(4);
					messageSendProtocol->sendNewVel(*messageSendProtocol->buffer, id, sX, sY, sZ);

					std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
					send(Clients[index].Connection, &packet[0], packet.size(), 0);
					delete messageSendProtocol;
				}

				if (messageProtocol->messageHeader.command_id == 4)
				{
					messageProtocol->receiveCard(*messageProtocol->buffer, Clients[index].playerDeck, cardsPlayed);
					sendCardIdToOther(index, 03, cardsPlayed.back()->id);

					sendMessageToClient(Clients[dealerId].Connection, 05, "Card played from player " + std::to_string(index) + " is " + cardsPlayed.back()->info);
					sendMessageToClient(Clients[otherId].Connection, 05, "Card played from player " + std::to_string(index) + " is " + cardsPlayed.back()->info);

					if (cardsPlayed.size() == 2)
					{
						int roundWinner = checkRound(cardsPlayed, dealerId, otherId);
						Clients[roundWinner].score += 1;
						roundNum++;

						if (roundNum == 10)
						{
							int gameWinner = -1;

							if (Clients[dealerId].score > Clients[otherId].score)
							{
								gameWinner = dealerId;
							}
							else if (Clients[dealerId].score < Clients[otherId].score)
							{
								gameWinner = otherId;
							}

							if (gameWinner == -1)
							{
								sendMessageToClient(Clients[dealerId].Connection, 06, "Game over.\nWinner is no one! Tied game! \nScores: \nPlayer " + std::to_string(dealerId) + ": " + std::to_string(Clients[dealerId].score) + "\nPlayer " + std::to_string(otherId) + ": " + std::to_string(Clients[otherId].score));
								sendMessageToClient(Clients[otherId].Connection, 06, "Game over.\nWinner is no one! Tied game! \nScores: \nPlayer " + std::to_string(dealerId) + ": " + std::to_string(Clients[dealerId].score) + "\nPlayer " + std::to_string(otherId) + ": " + std::to_string(Clients[otherId].score));
							}
							else
							{
								sendMessageToClient(Clients[dealerId].Connection, 06, "Game over.\nWinner is player " + std::to_string(gameWinner) + "\nScores: \nPlayer " + std::to_string(dealerId) + ": " + std::to_string(Clients[dealerId].score) + "\nPlayer " + std::to_string(otherId) + ": " + std::to_string(Clients[otherId].score));
								sendMessageToClient(Clients[otherId].Connection, 06, "Game over.\nWinner is player " + std::to_string(gameWinner) + "\nScores: \nPlayer " + std::to_string(dealerId) + ": " + std::to_string(Clients[dealerId].score) + "\nPlayer " + std::to_string(otherId) + ": " + std::to_string(Clients[otherId].score));
							}
						}
						else
						{
							sendMessageToClient(Clients[dealerId].Connection, 07, "Winner and advantage this round goes to " + std::to_string(roundWinner) + "\nCurrent Scores: \nPlayer " + std::to_string(dealerId) + ": " + std::to_string(Clients[dealerId].score) + "\nPlayer " + std::to_string(otherId) + ": " + std::to_string(Clients[otherId].score));
							sendMessageToClient(Clients[otherId].Connection, 07, "Winner and advantage this round goes to " + std::to_string(roundWinner) + "\nCurrent Scores: \nPlayer " + std::to_string(dealerId) + ": " + std::to_string(Clients[dealerId].score) + "\nPlayer " + std::to_string(otherId) + ": " + std::to_string(Clients[otherId].score));

							MessageProtocol* deleteCardProtocol = new MessageProtocol();
							deleteCardProtocol->createBuffer(4);
							deleteCardProtocol->removeCardFromTable(*deleteCardProtocol->buffer, 8, cardsPlayed[0]->id);
							std::vector<char> packet = deleteCardProtocol->buffer->mBuffer;
							send(Clients[cardsPlayed[0]->playerId].Connection, &packet[0], packet.size(), 0);
							delete deleteCardProtocol;

							MessageProtocol* deleteCardProtocol1 = new MessageProtocol();
							deleteCardProtocol1->createBuffer(4);
							deleteCardProtocol1->removeCardFromTable(*deleteCardProtocol1->buffer, 8, cardsPlayed[1]->id);
							std::vector<char> packet1 = deleteCardProtocol1->buffer->mBuffer;
							send(Clients[cardsPlayed[1]->playerId].Connection, &packet1[0], packet1.size(), 0);
							delete deleteCardProtocol1;

							std::cout << "SEND" << std::endl;

							cardsPlayed.clear();
						}
					}
				}

				packLength = 0;
				packet.clear();
				delete messageProtocol;
			}
		}
	}
}

int main()
{
	srand(time(NULL));
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBox(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	//Socket addres info

	SOCKADDR_IN addr;

	int addrlen = sizeof(addr);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
	addr.sin_port = htons(1234567);
	addr.sin_family = AF_INET; //IPv4 

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Creates socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); // Binds the address to the socket
	listen(sListen, SOMAXCONN); //Listen the socket

	std::cout << "Networking Project #3 - Veronika Kotckovich, Jenny Moon, Ryan O'Donnell" << std::endl;
	std::cout << "Server started" << std::endl;

	//To accept a connection
	SOCKET newConnection; //Socket to hold the client's connection
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
		if (newConnection == 0)
		{
			std::cout << "Failed to accept the client's connection." << std::endl;

		}
		else
		{
			std::cout << "New client Connected!" << std::endl;

			sendMessageToClient(newConnection, 0, "Waiting for another player...");


			Clients[i].Connection = newConnection;
			lobby.push_back(clientsCounter);
			clientsCounter++;
			std::cout << "Lobby size: " + std::to_string(lobby.size()) << std::endl;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)handleClients, (LPVOID)(i), NULL, NULL); //Create a thread
		}
	}
	system("pause");
	return 0;
}


void sendDeckToClient(int plyerID, std::vector <cCard*> theDeck)
{
	MessageProtocol* messageSendProtocol = new MessageProtocol();
	messageSendProtocol->messageHeader.command_id = 01;
	messageSendProtocol->createBuffer(4);
	messageSendProtocol->sendDeck(*messageSendProtocol->buffer, theDeck);

	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;

	send(Clients[plyerID].Connection, &packet[0], packet.size(), 0);

	delete messageSendProtocol;
}


//sendMessageOthersInGroup
//
//Purpouse: Sends server message to the curent conection
//
//@param: connection Id, ommand id, the message to be sent
//@return: void

void sendMessageToClient(SOCKET theConnection, int id, std::string message)
{
	MessageProtocol* messageSendProtocol = new MessageProtocol();
	messageSendProtocol->messageHeader.command_id = id;
	messageSendProtocol->messageBody.message = message;
	messageSendProtocol->createBuffer(4);
	messageSendProtocol->sendMessage(*messageSendProtocol->buffer, id);

	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;

	send(theConnection, &packet[0], packet.size(), 0);

	delete messageSendProtocol;
}

void sendCardIdToOther(int clientIndex, int id, int cardId)
{
	MessageProtocol* messageSendProtocol = new MessageProtocol();
	messageSendProtocol->messageHeader.command_id = id;

	messageSendProtocol->createBuffer(4);
	messageSendProtocol->sendCardId(*messageSendProtocol->buffer, id, cardId);
	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
	for (int i = 0; i < clientsCounter; i++)
	{
		if (clientIndex == i)
		{
			continue;
		}
		else
		{
			send(Clients[i].Connection, &packet[0], packet.size(), 0);
		}
	}
	delete messageSendProtocol;
}

//OLD
////sendMessageOthersInGroup
////
////Purpouse: Send message to all clients in the chat room including the curent conection
////
////@param:  room name, command id, the message to be sent
////@return: void
//
//void sendMessageToAllInGroup(std::string groupName, int id, std::string message)
//{
//	MessageProtocol* messageSendProtocol = new MessageProtocol();
//	messageSendProtocol->messageHeader.command_id = id;
//
//	messageSendProtocol->messageBody.message = message;
//	messageSendProtocol->createBuffer(4);
//	messageSendProtocol->sendMessage(*messageSendProtocol->buffer);
//	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
//	for (int i = 0; i < clientsCounter; i++)
//	{
//		if (Clients[i].room == groupName)
//		{
//			send(Clients[i].Connection, &packet[0], packet.size(), 0);
//		}
//		else 
//		{
//			continue;
//		}
//	}
//	delete messageSendProtocol;
//}
//
////sendMessageOthersInGroup
////
////Purpouse: Send message to all other clients in the chat room exept the curent conection
////
////@param: connection Id, room name, command id, the message to be sent
////@return: void
//
//void sendMessageOthersInGroup(int clientIndex,  std::string roomName, int id, std::string message)
//{
//	MessageProtocol* messageSendProtocol = new MessageProtocol();
//	messageSendProtocol->messageHeader.command_id = id;
//
//	messageSendProtocol->messageBody.message = message;
//	messageSendProtocol->createBuffer(4);
//	messageSendProtocol->sendMessage(*messageSendProtocol->buffer);
//	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
//	for (int i = 0; i < clientsCounter; i++)
//	{
//		if (clientIndex == i) 
//		{
//			continue;
//		}
//		if (Clients[i].room == roomName)
//		{
//			send(Clients[i].Connection, &packet[0], packet.size(), 0);
//		}
//	}
//	delete messageSendProtocol;
//}

//void sendResultToClient(SOCKET theConnection, int id, int winner)
//{
//	MessageProtocol* messageSendProtocol = new MessageProtocol();
//	messageSendProtocol->messageHeader.command_id = id;
//	messageSendProtocol->messageBody.message = "Winner of this round is " + std::to_string(winner);
//	messageSendProtocol->createBuffer(4);
//	messageSendProtocol->sendResults(*messageSendProtocol->buffer, id);
//
//	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
//
//	send(theConnection, &packet[0], packet.size(), 0);
//
//	delete messageSendProtocol;
//}
