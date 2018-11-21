//cLightManager.cpp
//
//purpose: Reads/Writes Jason file and creates a vecotor of lights e
#include "cLightManager.h"
#include <fstream>
#include <iostream>
#include "global.h"
#include <iomanip> 

cLightManager::cLightManager()
{
	this->numberOfLights = 12;
}

cLightManager::~cLightManager()
{
	for (unsigned int i = 0; i != vecLights.size(); i++)
	{
		delete vecLights[i];
	}
}

void cLightManager::TurnOnLight(int index, bool isOn)
{
	vecLights[index]->setLightOn(isOn);
}
void cLightManager::ChangeLightType(std::string lightType, int index) {

}

void cLightManager::fromJSON(const nlohmann::json& j, std::string theLightID, cLight* newLight)
{
	newLight->name = theLightID;
	j[theLightID]["positionX"].get_to(newLight->position.x);
	j[theLightID]["positionY"].get_to(newLight->position.y);
	j[theLightID]["positionZ"].get_to(newLight->position.z);
	j[theLightID]["positionW"].get_to(newLight->position.w);

	j[theLightID]["diffuseX"].get_to(newLight->diffuse.x);
	j[theLightID]["diffuseY"].get_to(newLight->diffuse.y);
	j[theLightID]["diffuseZ"].get_to(newLight->diffuse.z);
	j[theLightID]["diffuseW"].get_to(newLight->diffuse.w);

	j[theLightID]["constAtt"].get_to(newLight->atten.x);
	j[theLightID]["linAtt"].get_to(newLight->atten.y);
	j[theLightID]["quadAtt"].get_to(newLight->atten.z);
	j[theLightID]["diffuseCuttOff"].get_to(newLight->atten.w);

	j[theLightID]["specularR"].get_to(newLight->specular.r);
	j[theLightID]["specularG"].get_to(newLight->specular.g);
	j[theLightID]["specularB"].get_to(newLight->specular.b);
	j[theLightID]["specularPower"].get_to(newLight->diffuse.w);

	j[theLightID]["directionX"].get_to(newLight->direction.x);
	j[theLightID]["directionY"].get_to(newLight->direction.y);
	j[theLightID]["directionZ"].get_to(newLight->direction.z);
	j[theLightID]["directionW"].get_to(newLight->direction.w);

	j[theLightID]["lightType"].get_to(newLight->param1.x);
	float innerA, outterA;
	j[theLightID]["innerAngle"].get_to(innerA);
	j[theLightID]["outerAngle"].get_to(outterA);
	newLight->setSpotConeAngles(innerA, outterA);
	j[theLightID]["param1W"].get_to(newLight->param1.w);

	j[theLightID]["isLightOn"].get_to(newLight->param2.x);
	j[theLightID]["param2Y"].get_to(newLight->param2.y);
	j[theLightID]["param2Z"].get_to(newLight->param2.z);
	j[theLightID]["param2W"].get_to(newLight->param2.w);
}


void cLightManager::createLights(std::string fileName) {
	
	std::ifstream i(fileName);
	if (!i.is_open()) {
		std::cout << "Can't open a lights.JSON file" << std::endl;
	}
	nlohmann::json j;
	i >> j;
	i.close();
	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		cLight* newLight = new cLight();
		std::string theLight = it.key();
		fromJSON(j, theLight, newLight);
		this->vecLights.push_back(newLight);
	}
	std::cout << "Lights are loaded" << std::endl;

}

void cLightManager::CopyLightValueToShader() {
	for (unsigned int i=0; i != this->numberOfLights; i++)
	{
		cLight* tempLight = vecLights[i];
		glUniform4f(vecLights[i]->position_Unilock, tempLight->position.x, tempLight->position.y, tempLight->position.z, tempLight->position.w);
		glUniform4f(tempLight->diffuse_Unilock, tempLight->diffuse.x, tempLight->diffuse.y, tempLight->diffuse.z, tempLight->diffuse.w);
		glUniform4f(tempLight->atten_Unilock, tempLight->atten.x, tempLight->atten.y, tempLight->atten.z, tempLight->atten.w);
		glUniform4f(tempLight->specular_Unilock, tempLight->specular.x, tempLight->specular.y, tempLight->specular.z, tempLight->specular.w);
		glUniform4f(tempLight->direction_Unilock, tempLight->direction.x, tempLight->direction.y, tempLight->direction.z, tempLight->direction.w);
		glUniform4f(tempLight->param1_Unilock, tempLight->param1.x, tempLight->param1.y, tempLight->param1.z, tempLight->param1.w);
		glUniform4f(tempLight->param2_Unilock, tempLight->param2.x, tempLight->param2.y, tempLight->param2.z, tempLight->param2.w);
	}
}

void cLightManager::LoadUniforms(int program)
{
	for (unsigned int i = 0; i != vecLights.size(); i++)
	{
		cLight* tempLight = vecLights[i];

		std::string index = std::to_string(i);
		std::string shaderLPosition = "theLights[" + index + "].position";
		std::string shaderLDiffuse = "theLights[" + index + "].diffuse";
		std::string shaderLAtten = "theLights[" + index + "].atten";
		std::string shaderLSpecular = "theLights[" + index + "].specular";
		std::string shaderLDirection = "theLights[" + index + "].direction";
		std::string shaderLParam1 = "theLights[" + index + "].param1";
		std::string shaderLParam2 = "theLights[" + index + "].param2";

		tempLight->position_Unilock = glGetUniformLocation(program, shaderLPosition.c_str());
		tempLight->diffuse_Unilock = glGetUniformLocation(program, shaderLDiffuse.c_str());
		tempLight->atten_Unilock = glGetUniformLocation(program, shaderLAtten.c_str());
		tempLight->specular_Unilock = glGetUniformLocation(program, shaderLSpecular.c_str());
		tempLight->direction_Unilock = glGetUniformLocation(program, shaderLDirection.c_str());
		tempLight->param1_Unilock = glGetUniformLocation(program, shaderLParam1.c_str());
		tempLight->param2_Unilock = glGetUniformLocation(program, shaderLParam2.c_str());
	}
}

void cLightManager::setLights(int program, std::string fileName) {
	if (this->vecLights.size() == 0)
	{
		this->createLights(fileName);
	}
	this->LoadUniforms(program);
	this->CopyLightValueToShader();
}

void cLightManager::saveLights(std::string fileName)
{
	std::ofstream o(fileName);
	nlohmann::json j;
	for (int i = 0; i < vecLights.size(); i++)
	{
		std::string theLightID = vecLights[i]->name;
		j[theLightID]["positionX"] = vecLights[i]->position.x;
		j[theLightID]["positionY"] = vecLights[i]->position.y;
		j[theLightID]["positionZ"] = vecLights[i]->position.z;
		j[theLightID]["positionW"] = vecLights[i]->position.w;

		j[theLightID]["diffuseX"] = vecLights[i]->diffuse.x;
		j[theLightID]["diffuseY"] = vecLights[i]->diffuse.y;
		j[theLightID]["diffuseZ"] = vecLights[i]->diffuse.z;
		j[theLightID]["diffuseW"] = vecLights[i]->diffuse.w;

		j[theLightID]["constAtt"] = vecLights[i]->atten.x;
		j[theLightID]["linAtt"] = vecLights[i]->atten.y;
		j[theLightID]["quadAtt"] = vecLights[i]->atten.z;
		j[theLightID]["diffuseCuttOff"] = vecLights[i]->atten.w;

		j[theLightID]["specularR"] = vecLights[i]->specular.r;
		j[theLightID]["specularG"] = vecLights[i]->specular.g;
		j[theLightID]["specularB"] = vecLights[i]->specular.b;
		j[theLightID]["specularPower"] = vecLights[i]->diffuse.w;

		j[theLightID]["directionX"] = vecLights[i]->direction.x;
		j[theLightID]["directionY"] = vecLights[i]->direction.y;
		j[theLightID]["directionZ"] = vecLights[i]->direction.z;
		j[theLightID]["directionW"] = vecLights[i]->direction.w;

		j[theLightID]["lightType"] = vecLights[i]->param1.x;
		j[theLightID]["innerAngle"] = vecLights[i]->param1.y;
		j[theLightID]["outerAngle"] = vecLights[i]->param1.z;
		j[theLightID]["param1W"] = vecLights[i]->param1.w;

		j[theLightID]["isLightOn"] = vecLights[i]->param2.x;
		j[theLightID]["param2Y"] = vecLights[i]->param2.y;
		j[theLightID]["param2Z"] = vecLights[i]->param2.z;
		j[theLightID]["param2W"] = vecLights[i]->param2.w;

	}
	std::cout << "Saving lights to JASON file - " << std::endl;
	o << std::setw(4) << j << std::endl;
	o.close();
}

cLight* cLightManager::findLightByFriendlyName(std::string theName) {

	cLight* theLight = nullptr;

	for (unsigned int index = 0; index != vecLights.size(); index++)
	{
		if (vecLights[index]->name == theName) {
			theLight = vecLights[index];
		}
	}

	return theLight;
}

