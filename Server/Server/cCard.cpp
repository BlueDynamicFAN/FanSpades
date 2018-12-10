#include "cCard.h"
#include <iostream>

cCard::cCard(int id, Suit suit, Rank rank)
{
	this->id = id;
	this->suit = suit;
	this->rank = rank;
}

std::vector<cCard*> createDeck()
{
	std::vector<cCard*> cards;


	int id = 0;

	// For each suit
	for(int s = 0; s < 4; s++)
	{
		// For each rank
		for(int r = 0; r < 5; r++)
		{
			// Add a card with the current suit and rank
			cards.push_back(new cCard(id, (cCard::Suit)s, (cCard::Rank)r));
			id++;
		}
	}

	return cards;
}

void checkRound(std::vector<cCard*> &cardsPlayed, int &dealerId, int &otherId)
{
	if (cardsPlayed[0]->suit == cardsPlayed[1]->suit)
	{
		if (cardsPlayed[0]->rank > cardsPlayed[1]->rank)
		{
			std::cout << "Player " << cardsPlayed[0]->playerId << " wins the round" << std::endl;
		}
		else
		{
			std::cout << "Player " << cardsPlayed[1]->playerId << " wins the round" << std::endl;
		}
	}
	else if (cardsPlayed[0]->rank == cCard::Suit::Heart || cardsPlayed[1]->rank == cCard::Suit::Heart)
	{
		if (cardsPlayed[0]->rank == cCard::Suit::Heart)
		{
			std::cout << "Player " << cardsPlayed[0]->playerId << " wins the round" << std::endl;
		}
		else
		{
			std::cout << "Player " << cardsPlayed[1]->playerId << " wins the round" << std::endl;
		}
	}
	else
	{

	}
}
