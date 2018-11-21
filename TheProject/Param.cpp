//Param.cpp
//
//purpose: Describes physical parameters of the entity (Needed for Physics project)
#include "Param.h"
#include <iostream>
Param::Param()
{
	this->theShape = shape::UNKNOWN;
	this->velocity = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);
	this->doesPhysincs = false;
	this->m_radius = 0.0f;
	this->m_side1 = 0.0f;
	this->m_side2 = 0.0f;
	this->m_side3 = 0.0f;
	this->m_width = 0.0f;
	this->m_height = 0.0f;
	this->m_depth = 0.0f;
}

Param::~Param()
{

}

Param::Param(shape s) : Param()
{
	this->theShape = s;
}

Param::Param(shape s, glm::vec3 size, glm::vec3 vel, glm::vec3 accel) : Param()
{
	this->theShape = s;
	this->velocity = vel;
	this->accel = accel;
	switch (s)
	{
	case SPHERE:
		this->m_radius = size.x;
		break;
	case TRIAN:
		this->m_side1 = size.x;
		this->m_side2 = size.y;
		this->m_side3 = size.z;
		break;
	case CUBE:
		this->m_width = size.x;
		this->m_height = size.y;
		this->m_depth = size.z;
		break;
	case UNKNOWN:
		break;
	}
}
void Param::setSize(glm::vec3 size)
{
	switch (this->theShape)
	{
	case SPHERE:
		this->m_radius = size.x;
		break;
	case TRIAN:
		this->m_side1 = size.x;
		this->m_side2 = size.y;
		this->m_side3 = size.z;
		break;
	case CUBE:
		this->m_width = size.x;
		this->m_height = size.y;
		this->m_depth = size.z;
		break;
	case UNKNOWN:
		break;
	}
}
void Param::setVelocity(glm::vec3 vel)
{
	this->velocity = vel;
}
void Param::setAccel(glm::vec3 accel)
{
	this->accel = accel;
}

glm::vec3 Param::getSize()
{
	glm::vec3 size = glm::vec3(0.0f);
	switch (this->theShape)
	{
	case SPHERE:
		size.x = this->m_radius;
		break;
	case TRIAN:
		size.x = this->m_side1;
		size.y = this->m_side2;
		size.z = this->m_side3;
		break;
	case CUBE:
		size.x = this->m_width;
		size.y = this->m_height;
		size.z = this->m_depth;
		break;
	case UNKNOWN:
		break;
	}
	return size;
}
glm::vec3 Param::getVelocity(void)
{
	return this->velocity;
}
glm::vec3 Param::getAccel(void)
{
	return this->accel;
}