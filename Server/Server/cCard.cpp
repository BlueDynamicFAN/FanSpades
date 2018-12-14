#include "cCard.h"
#include <iostream>

std::string Suits[] = { "Spade", "Heart", "Club", "Diamond" };
std::string Ranks[] = { "Ace", "Two", "Three", "Four", "Five" };
cCard::cCard(int id, Suit suit, Rank rank)
{
	this->id = id;
	this->suit = suit;
	this->rank = rank;
	this->info = Ranks[rank] + " of " + Suits[suit];
}

std::vector<cCard*> createDeck()
{
	std::vector<cCard*> cards;

	int id = 0;

	// For each suit
	for (int s = 0; s < 4; s++)
	{
		// For each rank
		for (int r = 0; r < 5; r++)
		{
			// Add a card with the current suit and rank
			cards.push_back(new cCard(id, (cCard::Suit)s, (cCard::Rank)r));
			id++;
		}
	}

	return cards;
}

int checkRound(std::vector<cCard*> &cardsPlayed, int &dealerId, int &otherId)
{
	int winner = -1;

	if (cardsPlayed[0]->suit == cardsPlayed[1]->suit)
	{
		if (cardsPlayed[0]->rank > cardsPlayed[1]->rank)
		{
			winner = cardsPlayed[0]->playerId;
		}
		else
		{
			winner = cardsPlayed[1]->playerId;
		}
	}
	else if (cardsPlayed[0]->rank == cCard::Suit::Spade || cardsPlayed[1]->rank == cCard::Suit::Spade)
	{
		if (cardsPlayed[0]->rank == cCard::Suit::Spade)
		{
			winner = cardsPlayed[0]->playerId;
		}
		else
		{
			winner = cardsPlayed[1]->playerId;
		}
	}
	else
	{
		winner = dealerId;
	}

	std::cout << "Player " << winner << " wins the round" << std::endl;

	if (winner != dealerId)
	{
		otherId = dealerId;
		dealerId = winner;
	}

	return winner;
}
