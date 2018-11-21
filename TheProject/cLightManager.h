#ifndef _cLightManager_HG_
#define _cLightManager_HG_

#include <nlohmann/json.hpp>
#include "cLight.h"

class cLightManager
{
public:
	cLightManager();
	~cLightManager();
	std::vector<cLight*> vecLights;
	unsigned int numberOfLights;

	void TurnOnLight(int index, bool isOn);
	void ChangeLightType(std::string lightType, int index);
	void fromJSON(const nlohmann::json& j, std::string theLight, cLight* newLight);

	void createLights(std::string fileName);
	void setLights(int program, std::string fileName);
	void LoadUniforms(int program);
	void saveLights(std::string fileName);
	cLight* findLightByFriendlyName(std::string theName);
	//Will do glUniforms calls
	void CopyLightValueToShader(void);

};

#endif //_cLightManager_HG_