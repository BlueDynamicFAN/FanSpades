#include "global.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "cMeshObject.h"
#include "VAOMeshManager.h"
#include "cLightManager.h"
#include "cLightHelper.h"
#include "cShaderManager.h"
#include <ctime>
#include <cstdlib>

std::vector<cMeshObject*> g_modelsToDraw;
unsigned int madelsToDraw = 1;

std::string vertex_shader_text;
std::string fragment_shader_text;

cShaderManager* pTheShaderManager;
cBasicTextureManager* g_pTheTextureManager = NULL;
cFlyCamera* g_FlyCamera = NULL;
VAOMeshManager* meshManager;
cLightManager* pLightManager;
cLight* activeLight = NULL;

void UpdateWindowTitle(GLFWwindow* window);
void lightDebugSpheres(int program);

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
	GLFWwindow* window;
	GLuint vertex_shader, fragment_shader;// program;
	pLightManager = new cLightManager();

	srand(static_cast <unsigned> (time(0)));

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1740, 1180, "Triple Project", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	// NOTE: OpenGL error checks have been omitted for brevity

	//***Create a shader manager
	pTheShaderManager = new cShaderManager();

	//Create texture manager
	::g_pTheTextureManager = new cBasicTextureManager();

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("myShader", vertexShader, fragmentShader))
	{
		std::cout << "Compiled shaders OK" << std::endl;
	}
	else {
		std::cout << "OHH No" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}
	//***End 
	
	//***creating a chader program
	GLuint program = pTheShaderManager->getIDFromFriendlyName("myShader");
	

	// Load the uniform location values (some of them, anyway)
	cShaderManager::cShaderProgram* pSP = ::pTheShaderManager->pGetShaderProgramFromFriendlyName("myShader");
	pSP->LoadUniformLocation("texture00");
	pSP->LoadUniformLocation("texture01");
	pSP->LoadUniformLocation("texture02");
	pSP->LoadUniformLocation("texture03");
	pSP->LoadUniformLocation("texture04");
	pSP->LoadUniformLocation("texture05");
	pSP->LoadUniformLocation("texture06");
	pSP->LoadUniformLocation("texture07");
	pSP->LoadUniformLocation("texBlendWeights[0]");
	pSP->LoadUniformLocation("texBlendWeights[1]");


	meshManager = new VAOMeshManager();
	loadModelsIntoScene(program);

	//lights
	pLightManager->setLights(program, "./assets/JSON/lights1.json"); 
	activeLight = pLightManager->vecLights[0];
	
	g_FlyCamera = new cFlyCamera(); 
	g_FlyCamera->loadParams("./assets/JSON/camera.json");


	////get MVP uniforms
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");
	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");

	cLightHelper* pLightHelper = new cLightHelper();

	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH); 
		glEnable(GL_DEPTH_TEST); 
		glEnable(GL_CULL_FACE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4x4 matView, matProjection;
		matProjection = IDENTITY;
		matView = IDENTITY;
		

		matProjection = glm::perspective(0.6f, ratio, 0.1f, 1000.0f);
		matView = glm::lookAt(g_FlyCamera->eye, g_FlyCamera->getAtInWorldSpace(), g_FlyCamera->up);
		
		glUniform3f(eyeLocation_location, g_FlyCamera->eye.x, ::g_FlyCamera->eye.y, ::g_FlyCamera->eye.z);

		for (unsigned int numObj = 0; numObj < g_modelsToDraw.size(); numObj++) {
			glm::mat4x4 matModel = IDENTITY;
			
			DrawObj(g_modelsToDraw[numObj], program, matModel);

			glUniformMatrix4fv(matView_location, 1, GL_FALSE, glm::value_ptr(matView));
			glUniformMatrix4fv(matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));
		}

		pLightManager->setLights(program, "./assets/JSON/lights1.json");

		lightDebugSpheres(program);

		UpdateWindowTitle(window);
		processKeys(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete meshManager;
	delete pLightManager;
	delete pTheShaderManager;
	delete g_pTheTextureManager;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void UpdateWindowTitle(GLFWwindow* window)
{
	std::stringstream ssTitle;
	ssTitle << activeLight->name << " - "<<activeLight->atten.x << " , "
		<< activeLight->atten.y << " , "
		<< activeLight->atten.z;

	glfwSetWindowTitle(window, ssTitle.str().c_str());
}

void lightDebugSpheres(int program)
{
	if (pLightManager->vecLights.size() != 0)
	{
		cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSph");
		pDebugSphere->bIsVisiable = true;
		pDebugSphere->setDiffuseColor(glm::vec3(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f));
		pDebugSphere->bUseVertexColour = false;
		pDebugSphere->setUniformScale(1.0f);
		for (int i = 0; i < pLightManager->vecLights.size(); i++)
		{

			pDebugSphere->position = pLightManager->vecLights[i]->position;
			DrawObj(pDebugSphere, program, IDENTITY);
		}
		//pDebugSphere->bIsVisiable = false;

		pDebugSphere->setUniformScale(0.1f);

		pDebugSphere->position = activeLight->position;
		pDebugSphere->setDiffuseColor(glm::vec3(1.0, 1.0, 1.0));

		const float ACCURANCY_OF_DISTANCE = 0.01;
		const float INFINITY_DISTANCE = 0.01;

	}
}