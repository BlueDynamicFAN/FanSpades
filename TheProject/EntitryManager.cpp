//purpose: Reads/Writes to JSON file and creates a vector of entities and a vector of mesh objects

#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip> 
#include <iostream>
#include "cMeshObject.h"

extern std::vector<cMeshObject*> g_modelsToDraw;


void loadEntitiesFromJASON(std::string fileName)
{
	if (fileName == "")
	{
		fileName = "./assets/JSON/entities2.json";
	}
	std::ifstream i(fileName.c_str());
	nlohmann::json j;
	i >> j;
	i.close();

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		std::string fName = it.key();
		shape shape;
		j[fName]["shape"].get_to(shape);

		cMeshObject* theMesh = new cMeshObject();
		theMesh->friendlyName = fName;
		theMesh->theParams = new Param(shape);
		sTextureInfo texture;

		//Seting entity public variable
		j[fName]["useJSON"].get_to(theMesh->useJSON);
		j[fName]["meshFileName"].get_to(theMesh->meshName);
		j[fName]["isWireframed"].get_to(theMesh->bIsWireFrame);
		j[fName]["bIsVisiable"].get_to(theMesh->bIsVisiable);
		j[fName]["noLight"].get_to(theMesh->noLight);
		j[fName]["bUseVertexColour"].get_to(theMesh->bUseVertexColour);
		j[fName]["doesPhysincs"].get_to(theMesh->theParams->doesPhysincs);

		if (j[fName]["textuteName"] != nullptr)
		{
			j[fName]["textuteName"].get_to(texture.name);
			j[fName]["textureStren"].get_to(texture.strength);
		}

		theMesh->vecTextures.push_back(texture);

		//Seting entity private variables which requre setters and arrays
		float scale;

		if (theMesh->theParams->doesPhysincs)
		{
			std::vector<float> accel = j[fName]["accel"];
			std::vector<float> vellocity = j[fName]["vellocity"];

			theMesh->theParams->setAccel(glm::vec3(accel[0], accel[1], accel[2]));
			theMesh->theParams->setVelocity(glm::vec3(vellocity[0], vellocity[1], vellocity[2]));
		}

		j[fName]["scale"].get_to(scale);
		std::vector<float> size = j[fName]["size"];
		std::vector<float> position = j[fName]["position"];
		std::vector<float> diffuse = j[fName]["diffuse"];
		std::vector<float> specular = j[fName]["specular"];
		std::vector<float> rotation = j[fName]["rotation"];


		theMesh->position = glm::vec3(position[0], position[1], position[2]);
		theMesh->setDiffuseColor(glm::vec3(diffuse[0], diffuse[1], diffuse[2]));
		theMesh->setAlphaTransparency(diffuse[3]);
		theMesh->setUniformScale(scale);
		theMesh->setSpecularPower(specular[3]);
		theMesh->setSpecularColour(glm::vec3(specular[0], specular[1], specular[2]));
		theMesh->setMeshOrientationQuat(glm::vec4(rotation[0], rotation[1],
			rotation[2], rotation[3]));
		std::cout << "X: " << rotation[0] << "Y: " << rotation[1] << "Z: " << rotation[2] << std::endl;
		theMesh->theParams->setSize(glm::vec3(size[0], size[1], size[2]));

		
		g_modelsToDraw.push_back(theMesh);
	}
}

void loadALLEntitiesToJASON(std::string fileName)
{
	if (fileName == "")
	{
		fileName = "./assets/JSON/entities2.json";
	}
	std::ofstream o(fileName);
	nlohmann::json j;

	for (int i = 0; i < g_modelsToDraw.size(); i++)
	{
		if (g_modelsToDraw[i]->useJSON == false)
		{
			continue;
		}
		std::string fName = g_modelsToDraw[i]->friendlyName;
		j[fName]["shape"] = g_modelsToDraw[i]->theParams->theShape;
		j[fName]["useJSON"] = g_modelsToDraw[i]->useJSON;
		j[fName]["meshFileName"] = g_modelsToDraw[i]->meshName;
		j[fName]["isWireframed"] = g_modelsToDraw[i]->bIsWireFrame;
		j[fName]["bIsVisiable"] = g_modelsToDraw[i]->bIsVisiable;
		j[fName]["noLight"] = g_modelsToDraw[i]->noLight;
		j[fName]["bUseVertexColour"] = g_modelsToDraw[i]->bUseVertexColour;
		j[fName]["doesPhysincs"] = g_modelsToDraw[i]->theParams->doesPhysincs;
		j[fName]["scale"] = g_modelsToDraw[i]->nonUniformScale.x;
		j[fName]["textuteName"] = g_modelsToDraw[i]->vecTextures[0].name;
		j[fName]["textureStren"] = g_modelsToDraw[i]->vecTextures[0].strength;


		std::vector <float> accel = { g_modelsToDraw[i]->theParams->getAccel().x, g_modelsToDraw[i]->theParams->getAccel().y, g_modelsToDraw[i]->theParams->getAccel().z };
		std::vector <float> velocity = { g_modelsToDraw[i]->theParams->getVelocity().x, g_modelsToDraw[i]->theParams->getVelocity().y, g_modelsToDraw[i]->theParams->getVelocity().z };

		std::vector <float> position = { g_modelsToDraw[i]->position.x, g_modelsToDraw[i]->position.y, g_modelsToDraw[i]->position.z };
		std::vector <float> diffuse = { g_modelsToDraw[i]->getDiffuseColor().r, g_modelsToDraw[i]->getDiffuseColor().g,
								g_modelsToDraw[i]->getDiffuseColor().b, g_modelsToDraw[i]->getDiffuseColor().w };

		std::vector <float> specular = { g_modelsToDraw[i]->getSpecularColour().r, g_modelsToDraw[i]->getSpecularColour().g,
								g_modelsToDraw[i]->getSpecularColour().b, g_modelsToDraw[i]->getSpecularColour().w };

		std::vector <float> rotation = { g_modelsToDraw[i]->getQOrientation().x, g_modelsToDraw[i]->getQOrientation().y,
												g_modelsToDraw[i]->getQOrientation().z, g_modelsToDraw[i]->getQOrientation().w };

		std::vector <float> size = { g_modelsToDraw[i]->theParams->getSize().x, g_modelsToDraw[i]->theParams->getSize().y,
												g_modelsToDraw[i]->theParams->getSize().z };

		j[fName]["accel"] = accel;
		j[fName]["vellocity"] = velocity;
		j[fName]["position"] = position;
		j[fName]["diffuse"] = diffuse;
		j[fName]["specular"] = specular;
		j[fName]["rotation"] = rotation;
		j[fName]["size"] = size;
	}

	std::cout << "Saving model to JASON file - " << fileName << std::endl;
	o << std::setw(4) << j << std::endl;
	o.close();
}
