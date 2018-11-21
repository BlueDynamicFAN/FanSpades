#include "Entity.h"

Entity::Entity()
{

	this->name = "";
	this->uniqueID = nextID;
	this->model = new cModelObj();
	this->theParams = nullptr;
}

Entity::~Entity()
{
	delete model;
	delete theParams;
}

Entity::Entity(std::string n, shape s) : Entity()
{
	this->name = n;
	this->model->friendlyName = n;
	this->theParams = new Param(s);
}

unsigned int Entity::getID()
{
	return this->uniqueID;
}

unsigned int Entity::nextID = Entity::START;