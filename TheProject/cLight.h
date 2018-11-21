#ifndef sLight_HG_
#define sLight_HG_
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp>

enum eLightType
{ //Based on shaders

	POINT_LIGHT,       // = 0
	SPOT_LIGHT,        // = 1
	DIRECTIONAL_LIGHT  // = 2

};


class cLight
{
public:
	std::string name;
	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular; //rgb = hightlight colour, w = power
	glm::vec4 atten; //x-constant; y=linear, z = quadratic, w--diffuse cut off
	glm::vec4 direction; //Spot, directional light;
	glm::vec4 param1; // x: lightType, y: inner angle; z: outer angle; z-TBD
	glm::vec4 param2; // x = 0 -- light off, 1 - is on

	cLight();
	~cLight();

	void setPosition(glm::vec4 pos);
	void setDiffuse(glm::vec4 dif);
	void setSpecular(glm::vec4 spec);
	
	void setConstAtten(float att);
	void setLinAtten(float att);
	void setQuadAtten(float att);
	void setDuffuseCutOff(float att);


	void setDirection(glm::vec4 dir);
	void setLightType(float param);
	void setInnerAngle(float param);
	void setOuterAngle(float param);
	void setLightOn(bool isOn);
	void setParam2(glm::vec4 param);

	int position_Unilock;
	int diffuse_Unilock;
	int specular_Unilock;
	int atten_Unilock;
	int direction_Unilock;
	int param1_Unilock;
	int param2_Unilock;

	void setLightType(std::string typeAsString);
	void setLightType(eLightType type);
	void setSpotConeAngles(float innerAngle, float outerAngle);

private:
	eLightType m_lightType;
};

#endif // !sLight_HG_