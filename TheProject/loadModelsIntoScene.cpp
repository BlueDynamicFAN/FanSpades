//loadModelsIntoScene.cpp
//
//purpose: Load mesh objects to the scene

#include "global.h"
#include "sDrawInfo.h"
#include "VAOMeshManager.h"

void setModelsParams();
void setTextures();


void loadModelsIntoScene(int program)
{
	setModelsParams(); // load models from assets/JSON

	//Load models manually
	{
		cMeshObject* model = new cMeshObject();
		model->position = glm::vec3(0.0f, 0.0f, 0.0f);
		//model->noLight = false;
		model->nonUniformScale = glm::vec3(1.0f, 1.0f, 1.0f);
		model->bIsVisiable = false;
		model->bIsWireFrame = true;
		model->friendlyName = "DebugSph";
		model->useJSON = false; //!! important
		model->meshName = "./assets/Models/Sphere_320_faces_xyz_n_uv.ply";
		g_modelsToDraw.push_back(model);
	}

	for (unsigned int index = 0; index != g_modelsToDraw.size(); index++) {
		sDrawInfo modelInfo;
		modelInfo.meshName = g_modelsToDraw[index]->meshName;
		if (meshManager->FindDrawInfoByModelName(modelInfo))
		{
			std::cout << modelInfo.meshName << " model was already loaded" << std::endl;
			continue;
		}
		if (!meshManager->LoadModelIntoVAO(modelInfo, program)) {
			std::cout << "Can't load" << g_modelsToDraw[index]->friendlyName << " mesh model" << std::endl;
		}
		else {
			std::cout << modelInfo.meshName << " model info is loaded" << std::endl;
		}
	}

	// Load the textures, too
	setTextures();
}


void setModelsParams()
{
	loadEntitiesFromJASON("./assets/JSON/entities2.json");
	std::cout << "Models are loaded" << std::endl;

}

void setTextures()
{
	::g_pTheTextureManager->SetBasePath("assets/textures");

	if (!::g_pTheTextureManager->Create2DTextureFromBMPFile("Stonewall15_512x512.bmp", true))
	{
		std::cout << "Didn't load texture" << std::endl;
	}
	::g_pTheTextureManager->Create2DTextureFromBMPFile("fabric.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("brick-wall.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("Plastic.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("wood.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("cracked.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("apple.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("fur.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("stones.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("Bubble-Texture.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("Dog_diffuse.bmp", true);

	cMeshObject* tab = findObjectByFriendlyName("Table");
	sTextureInfo texture;
	texture.name = "cracked.bmp";
	texture.strength = 0.7;
	tab->vecTextures.push_back(texture);
}


cMeshObject* findObjectByFriendlyName(std::string theName) {

	cMeshObject* pTheObjectWeFound = nullptr;

	for (unsigned int index = 0; index != g_modelsToDraw.size(); index++)
	{
		if (g_modelsToDraw[index]->friendlyName == theName) {
			pTheObjectWeFound = g_modelsToDraw[index];
		}
	}

	return pTheObjectWeFound;
}
cMeshObject* findObjectByUniqueID(unsigned int ID) {
	for (unsigned int index = 0; index != g_modelsToDraw.size(); index++)
	{
		if (g_modelsToDraw[index]->getID() == ID) {
			return g_modelsToDraw[index];
		}
	}

	return nullptr;
}