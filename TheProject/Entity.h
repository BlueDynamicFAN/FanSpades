#ifndef Entity_HG
#define Entity_HG

#include "cModelObj.h"
#include "Param.h"
#include <string>


class Entity {
public:
	Entity();
	Entity(std::string n, shape s);

	~Entity();

	std::string name;
	cModelObj* model;
	Param* theParams;

	unsigned int getID();

private:
	unsigned int uniqueID;
	static unsigned int nextID;
	static const unsigned int START = 0;
};

#endif