#ifndef _cModelObj_HG_
#define _cModelObj_HG_

#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>
#include "Param.h"

struct sTextureInfo 
{ 
      std::string name = ""; 
      int cachedTextureID; 	// Set to -1 by default
      float strength;		// Set to 0.0f by default
};

class Collision
{
public:
	glm::vec3 positionWhenCollide;
	std::string colideWithName;
};

class cMeshObject {
	
public:
	cMeshObject();
	~cMeshObject();
	glm::quat getQOrientation(void) { return this->m_meshQOrientation; };
	void setMeshOrientationQuat(glm::vec4 newAngles);
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
	glm::vec3 position;

	glm::vec3 nonUniformScale;
	glm::vec3 color;

	std::string friendlyName;
	std::string meshName;

	bool bIsWireFrame;
	bool bIsVisiable;
	bool bUseVertexColour;
	bool noLight;
	bool useJSON;
	glm::vec4 materialDiffuse;
	glm::vec4 materialSpecular;
	void setDiffuseColor(glm::vec3 newDiffuse);
	void setAlphaTransparency(float newAlpha);
	void setSpecularColour(glm::vec3 colourRGB);
	void setSpecularPower(float specPower);
	void setUniformScale(float scale);

	glm::vec4 getDiffuseColor();
	glm::vec4 getSpecularColour();

	inline unsigned int getID() {
		return this->uniqueID;
	}

	// Textures...
	std::vector<sTextureInfo> vecTextures;

	Collision* collision;
	Param* theParams;
private:
	glm::quat m_meshQOrientation;
	unsigned int uniqueID;
	static unsigned int nextModelID;
	static const unsigned int START = 0;
};

#endif // !_cModelObj_HG_
