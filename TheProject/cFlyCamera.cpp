#include "cFlyCamera.h"
//#include <glm/glm.hpp>
//#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>


cFlyCamera::cFlyCamera()
{
	this->eye = glm::vec3(-100.0f, 10.0f, +30.0f);
	//initial at 1 unit away
	this->at = glm::vec3(0.0f, 0.0f, 1.0f);

	this->frontOfCamera = glm::vec3(0.0f, 0.0f, -1.0f);

	//set initial orientation
	this->qOrientation = glm::quat(glm::vec3(0.0f));
	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	
	this->movementSpeed = 1.0f;

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
}


cFlyCamera::~cFlyCamera()
{
}

void cFlyCamera::setEyeLocation()
{

}

void cFlyCamera::loadParams(std::string filename)
{
	std::ifstream i(filename);
	if (!i.is_open()) {
		std::cout << "Can't open a camera file" << std::endl;
	}

	nlohmann::json j;
	i >> j;
	i.close();
	std::vector<float> location = j["Camera"]["location"];
	std::vector<float> at = j["Camera"]["at"];
	std::vector<float> up = j["Camera"]["up"];
	this->eye = glm::vec3(location[0], location[1], location[2]);
	this->at = glm::vec3(at[0], at[1], at[2]);
	this->up = glm::vec3(up[0], up[1], up[2]);
	this->frontOfCamera = glm::vec3(at[0], at[1], at[2]);
	std::cout << "eye " << this->eye.x << " " << this->eye.y << " " << this->eye.z << std::endl;
	std::cout << "at " << this->at.x << " " << this->at.y << " " << this->at.z << std::endl;
	std::cout << "up " << this->up.x << " " << this->up.y << " " << this->up.z << std::endl;
	std::cout << "CameraLoaded" << std::endl;
}
void cFlyCamera::saveParams(std::string filename)
{
	std::ofstream o(filename);
	nlohmann::json j;
	std::vector<float> location = { this->eye.x, this->eye.y, this->eye.z };
	std::vector<float> at = { this->at.x, this->at.y, this->at.z };
	std::vector<float> up = { this->up.x, this->up.y, this->up.z };

	j["Camera"]["up"] = up;
	j["Camera"]["at"] = at;
	j["Camera"]["location"] = location;
	o << std::setw(4) << j << std::endl;
	o.close();
	std::cout << "Camera params are saved" << std::endl;
}

glm::vec3 cFlyCamera::getAtInWorldSpace(void)
{
	// The "At" is relative to the where the camera is...
	// So just add the "At" to the current camera location
	return this->eye + this->at;
}

glm::vec3 cFlyCamera::getCameraDirection(void)
{
	this->m_UpdateAtFromOrientation();

	return this->at;
}

void cFlyCamera::MoveForward_Z(float amount)
{
	//We take the vecore from the eye to at 
	//Add this forward velocity along this direction


	glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	// Make direction a "unit length"
	direction = glm::normalize(direction);

	// Generate a "forward" adjustment value 
	glm::vec3 amountToMove = direction * amount;

	// Add this to the eye
	this->eye += amountToMove;
	//this->m_UpdateAtFromOrientation();
	//this->m_UpdateUpFromOrientation();

	return;
}
void cFlyCamera::MoveLeftRight_X(float amount)
{
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	// 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->up);

	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	this->eye += vecAmountToMove;

	return;
}
void cFlyCamera::MoveUpDown_Y(float amount)
{
	glm::vec3 vecAmountToMove = glm::normalize(this->up) * amount;

	this->eye += vecAmountToMove;
}

void cFlyCamera::Pitch_UpDown(float angleDegree) // around x
{
	//Adjuct orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(angleDegree, 0.0f, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
}

void cFlyCamera::Yaw_LeftRight(float angleDegree)
{
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, angleDegree, 0.0f), true);
	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
}
void cFlyCamera::Roll_ClockWise(float angleDegree)
{
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, angleDegree), true);
	m_UpdateAtFromOrientation();
	m_UpdateUpFromOrientation();
}

void cFlyCamera::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/)
{
	if (bIsDegrees)
	{
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
			glm::radians(newAnglesEuler.y),
			glm::radians(newAnglesEuler.z));
	}

	this->qOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/)
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
	this->qOrientation *= rotationAdjust;
	this->qOrientation = this->qOrientation * rotationAdjust;

	return;
}

void cFlyCamera::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/)
{
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationQ(glm::quat adjOrientQ)
{
	this->qOrientation *= adjOrientQ;
	return;
}

void cFlyCamera::m_UpdateAtFromOrientation()
{
	glm::mat4 matRoation = glm::mat4(this->qOrientation);
	glm::vec4 frontOfTheCamera = glm::vec4(this->frontOfCamera, 1.0f);
	glm::vec4 newAT = matRoation * frontOfTheCamera;
	this->at = glm::vec3(newAT.x, newAT.y, newAT.z);
	return;
}

void cFlyCamera::m_UpdateUpFromOrientation(void)
{
	glm::mat4 matRotation = glm::mat4(this->qOrientation);

	glm::vec4 upVector = glm::vec4(this->m_upIsYVector, 1.0f);

	glm::vec4 newUp = matRotation * upVector;

	// Update the "At"
	this->up = glm::vec3(newUp);

	return;
}
