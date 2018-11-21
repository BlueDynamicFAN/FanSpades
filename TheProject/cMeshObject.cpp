#include "cMeshObject.h"
#include <iostream>

cMeshObject::cMeshObject() {
	this->position = glm::vec3(0.0f);
	this->setMeshOrientationEulerAngles(glm::vec3(0.0f));
	this->m_meshQOrientation = glm::quat(glm::vec3(0.0f));
	this->nonUniformScale = glm::vec3(1.0f);
	this->bIsWireFrame = false;
	this->bUseVertexColour = false;
	this->noLight = false;
	this->bIsVisiable = true;
	this->uniqueID = nextModelID;

	this->theParams = new Param();
	this->collision = new Collision();
	this->collision->colideWithName = "";

	this->materialDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	nextModelID++;
}

unsigned int cMeshObject::nextModelID = cMeshObject::START;

cMeshObject::~cMeshObject() {
	delete this->theParams;
}

void cMeshObject::setUniformScale(float scale)
{
	this->nonUniformScale = glm::vec3(scale, scale, scale);
	return;
}

void cMeshObject::setDiffuseColor(glm::vec3 newDiffuse)
{
	this->materialDiffuse = glm::vec4(newDiffuse, this->materialDiffuse.a);
	return;
}

void cMeshObject::setAlphaTransparency(float newAlpha)
{
	this->materialDiffuse.a = newAlpha;
	return;
}

void cMeshObject::setSpecularColour(glm::vec3 colourRGB)
{
	this->materialSpecular = glm::vec4(colourRGB, this->materialSpecular.a);
	return;
}

void cMeshObject::setSpecularPower(float specPower)
{
	this->materialSpecular.a = specPower;
	return;
}

glm::vec4 cMeshObject::getDiffuseColor()
{
	return this->materialDiffuse;
}
glm::vec4 cMeshObject::getSpecularColour()
{
	return this->materialSpecular;
}



void cMeshObject::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
			glm::radians(newAnglesEuler.y),
			glm::radians(newAnglesEuler.z));
	}

	this->m_meshQOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cMeshObject::setMeshOrientationQuat(glm::vec4 newAngles)
{
	this->m_meshQOrientation.x = newAngles.x;
	this->m_meshQOrientation.y = newAngles.y;
	this->m_meshQOrientation.z = newAngles.z;
	this->m_meshQOrientation.w = newAngles.w;
	return;
}

void cMeshObject::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cMeshObject::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x),
			glm::radians(adjAngleEuler.y),
			glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->m_meshQOrientation *= rotationAdjust;

	return;
}

void cMeshObject::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cMeshObject::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->m_meshQOrientation *= adjOrientQ;
	std::cout << "X: " << m_meshQOrientation.x << "Y: " << m_meshQOrientation.y << "Z: " << m_meshQOrientation.z << std::endl;
	return;
}