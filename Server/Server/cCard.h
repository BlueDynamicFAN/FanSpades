#ifndef _cCard_HG_
#define _cCard_HG_

#include <string>
#include <vector>

struct Position
{
	float x;
	float y;
	float z;
};

extern std::vector<Position> positions;

class cCard
{
public:
	enum Suit { Spade, Heart, Club, Diamond };
	enum Rank { Ace, One, Two, Three, Four, Five };
	
	cCard(int id, Suit suit, Rank rank);

	int id;
	Suit suit;
	Rank rank;
	int playerId;
	Position position;
};

#endif
