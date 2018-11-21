#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

class cFlyCamera
{
public:
	//Use there to alow us to use LookAt() transform
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;

	glm::quat qOrientation;


	cFlyCamera();
	~cFlyCamera();
	
	void setEyeLocation();
	void loadParams(std::string filename);
	void saveParams(std::string filename);

	void MoveForward_Z(float amount);
	void MoveLeftRight_X(float amount);
	void MoveUpDown_Y(float amount);

	void Pitch_UpDown(float angleDegree); // around x
	void Yaw_LeftRight(float angleDegree); //
	void Roll_ClockWise(float angleDegree);

	glm::quat getQOrientation(void) { return this->qOrientation; };
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);

	glm::vec3 getAtInWorldSpace(void);
	glm::vec3 getCameraDirection(void);
	glm::vec3 frontOfCamera;

private:
	glm::vec3 m_upIsYVector;
	void m_UpdateAtFromOrientation();
	void m_UpdateUpFromOrientation(void);
	float movementSpeed;
};

