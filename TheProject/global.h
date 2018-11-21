#ifndef GLOBAL_HG
#define GLOBAL_HG

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> 
#include <iostream>
#include <string>
#include <vector>
#include "VAOMeshManager.h"
#include "cMeshObject.h"
#include "sPlyVertex.h"
#include "sPlyTriangle.h"
#include "Camera.h"
#include "cFlyCamera.h"
#include "TextureManager/cBasicTextureManager.h"

//extern Camera* camera;
extern cFlyCamera* g_FlyCamera;
extern VAOMeshManager* meshManager;
extern cBasicTextureManager* g_pTheTextureManager;

const glm::mat4x4 IDENTITY = glm::mat4(1.0f);
extern sPlyVertex* g_pPlyVertexArray;
extern sPlyTriangle* g_pPlyFaceArray;
extern unsigned int g_vertexNum;
extern unsigned int g_faceNum;
extern unsigned int g_unrolledVertNum;
//extern glm::vec3 g_CameraEye;
extern std::vector<cMeshObject*> g_modelsToDraw;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processKeys(GLFWwindow* window);
void loadModelsIntoScene(int);
void loadEntitiesFromJASON(std::string fileName);
void loadALLEntitiesToJASON(std::string fileName);
cMeshObject* findObjectByFriendlyName(std::string theName);
cMeshObject* findObjectByUniqueID(unsigned int ID);
void DrawObj(cMeshObject* currentObj, int program, glm::mat4x4 matModel);

#endif // !GLOBAL_HG

