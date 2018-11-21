#include "cLight.h"



cLight::cLight()
{
	this->name = "";
	this->position = glm::vec4(0.0f);
	this->diffuse = glm::vec4(0.0f);
	this->specular;// = glm::vec4(0.0f);
	this->atten = glm::vec4(0.0f);
	this->direction;// = glm::vec4(0.0f);
	this->param1;// = glm::vec4(0.0f);
	this->param2 = glm::vec4(0.0f);
}


cLight::~cLight()
{
}

void cLight::setPosition(glm::vec4 pos)
{
	this->position = pos;
}
void cLight::setDiffuse(glm::vec4 dif)
{
	this->diffuse = dif;
}
void cLight::setSpecular(glm::vec4 spec)
{
	this->specular = spec;
}

void cLight::setConstAtten(float att)
{
	this->atten.x = att;
}

void cLight::setLinAtten(float att)
{
	this->atten.y = att;
}

void cLight::setQuadAtten(float att)
{
	this->atten.z = att;
}

void cLight::setDuffuseCutOff(float att)
{
	this->atten.w = att;
}

void cLight::setDirection(glm::vec4 dir)
{
	this->direction = dir;
}

//x: lightType, y: inner angle; z: outer angle; z-TBD
void cLight::setLightType(float param)
{
	this->param1.x = param;
}

void cLight::setInnerAngle(float param)
{
	this->param1.y = param;
}

void cLight::setOuterAngle(float param)
{
	this->param1.z = param;
}

void cLight::setParam2(glm::vec4 param)
{
	this->param2 = param;
}

void cLight::setLightOn(bool isOn)
{
	this->param2.x = isOn;
}

void cLight::setLightType(eLightType type)
{
	switch (type)
	{
	case POINT_LIGHT:
		this->param1.x = 0;
		break;
	case SPOT_LIGHT:
		this->param1.x = 1;
		break;
	case DIRECTIONAL_LIGHT:
		this->param1.x = 2;
		break;
	default: //point light is default
		this->param1.x = 0;
		break;
	}
}

void cLight::setSpotConeAngles(float innerAngle, float outerAngle)
{
	/*if (innerAngle < outerAngle)
	{
		float temp = innerAngle;
		innerAngle = outerAngle;
		outerAngle = temp;
	}*/
	this->param1.y = innerAngle;
	this->param1.z = outerAngle;
}
