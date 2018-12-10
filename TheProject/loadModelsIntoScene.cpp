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

	::g_pTheTextureManager->Create2DTextureFromBMPFile("S1.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("S2.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("S3.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("S4.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("S5.bmp", true);

	::g_pTheTextureManager->Create2DTextureFromBMPFile("H1.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("H2.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("H3.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("H4.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("H5.bmp", true);

	::g_pTheTextureManager->Create2DTextureFromBMPFile("C1.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("C2.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("C3.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("C4.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("C5.bmp", true);

	::g_pTheTextureManager->Create2DTextureFromBMPFile("D1.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("D2.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("D3.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("D4.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("D5.bmp", true);

	::g_pTheTextureManager->Create2DTextureFromBMPFile("Blue.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("Green.bmp", true);
	::g_pTheTextureManager->Create2DTextureFromBMPFile("Red.bmp", true);


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
