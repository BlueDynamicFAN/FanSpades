#include "global.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include "Buffer.h"
#include "MessageProtocol.h"
#include <windows.h>

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

std::vector<int> playerCards;
std::vector<int> otherCards;
unsigned int activeId = 0;
unsigned int cardToMove = 0;
bool moveTheCard = false;
glm::vec3 newPosition = glm::vec3(0.0f);

SOCKET Connection;
int commandID;
bool run = true;

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

bool isCardPlayed = false;
bool isGameOver = false;

bool PRESS = false;

int otherCardId = -1;

void UpdateWindowTitle(GLFWwindow* window);
void lightDebugSpheres(int program);
void updateCardPosition(double deltaTime, int id, glm::vec3 newPosition);

std::string textureNames[] = { "S1.bmp", "S2.bmp", "S3.bmp", "S4.bmp", "S5.bmp", "H1.bmp", "H2.bmp", "H3.bmp", "H4.bmp", "H5.bmp", "C1.bmp", "C2.bmp", "C3.bmp", "C4.bmp", "C5.bmp", "D1.bmp", "D2.bmp", "D3.bmp", "D4.bmp", "D5.bmp" };

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}


//clientThread()
//
//Purpose: Handle a client thread; receiving messages from the server
//@param: void
//@return: void
void clientThread()
{
	std::vector<char> packet(512);
	int packLength;
	while (run)
	{
		if ((packLength = recv(Connection, &packet[0], packet.size(), NULL)) < 1) {
			closesocket(Connection);
			WSACleanup();
			run = false;
		}

		else
		{
			MessageProtocol* messageProtocol = new MessageProtocol();
			messageProtocol->createBuffer(512);
			messageProtocol->buffer->mBuffer = packet;
			messageProtocol->readHeader(*messageProtocol->buffer);

			messageProtocol->buffer->resizeBuffer(messageProtocol->messageHeader.packet_length);
			std::cout << "Command ID " << messageProtocol->messageHeader.command_id << std::endl;
			if (messageProtocol->messageHeader.command_id == 1)
			{
				messageProtocol->receiveDeck(*messageProtocol->buffer, playerCards);
				std::cout << "DECK RECIEVED" << std::endl;
			}
			else if (messageProtocol->messageHeader.command_id == 3)
			{
				messageProtocol->receiveCardId(*messageProtocol->buffer, otherCardId);

				std::cout << otherCardId << std::endl;
				g_modelsToDraw[10]->vecTextures[0].name = textureNames[otherCardId];
			}
			else if (messageProtocol->messageHeader.command_id == 4)
			{
				float x, y, z;
				int id;
				messageProtocol->receiveNewVelocity(*messageProtocol->buffer, id, x, y, z);
				std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
				findObjectByUniqueID(id)->initialPosition = findObjectByUniqueID(id)->position;
				//std::cout << "initialPosition " << findObjectByUniqueID(id)->initialPosition.x << " " <<
				//	findObjectByUniqueID(id)->initialPosition.y << " " <<
				//	findObjectByUniqueID(id)->initialPosition.z << " " << std::endl;

				//std::cout << "position " << findObjectByUniqueID(id)->position.x << " " <<
				//	findObjectByUniqueID(id)->position.y << " " <<
				//	findObjectByUniqueID(id)->position.z << " " << std::endl;

				//findObjectByUniqueID(id)->position = glm::vec3(x, y, z);
				cardToMove = id;
				if (fabs(z) > 35.0f) //sometimes z => 1000, so a card just fly away
				{
					z = 0.0f;
				}
				newPosition = glm::vec3(x, y, z);
				moveTheCard = true;

			}
			else if (messageProtocol->messageHeader.command_id == 8)
			{
				//std::cout << "GET" << std::endl;
				//int tempId = -1;
				//messageProtocol->receiveCardId(*messageProtocol->buffer, tempId);
				//std::cout << "TEMP ID " << tempId << std::endl;
				//findObjectByUniqueID(tempId)->bIsVisiable = false;
				//g_modelsToDraw[10]->vecTextures[0].name = "Red.bmp";
			}
			else {
				messageProtocol->receiveMessage(*messageProtocol->buffer);
				std::cout << messageProtocol->messageBody.message << std::endl;
				commandID = messageProtocol->messageHeader.command_id;

				if (commandID == 07)
				{
					isCardPlayed = false;
				}
				else if (commandID == 06)
				{
					isGameOver = true;
				}
			}
			delete messageProtocol;
			//packet.clear();
		}
	}
}


int main(void)
{
	//SOCKET CONNEACTION
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBox(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	//Setting Socket address
	SOCKADDR_IN addr;
	int sizeofadr = sizeof(addr);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
	addr.sin_port = htons(1234567);
	addr.sin_family = AF_INET; //IPv4 

	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Creates connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofadr) != 0)
	{
		MessageBox(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
	}
	std::cout << "Connected!" << std::endl;

	/*MessageProtocol* messageProtocol = new MessageProtocol();
	messageProtocol->createBuffer(256);*/

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientThread, NULL, NULL, NULL); //Create a thread

	MessageProtocol* messageSendProtocol = new MessageProtocol();
	//********************************************

	GLFWwindow* window;
	GLuint vertex_shader, fragment_shader;// program;
	pLightManager = new cLightManager();

	srand(static_cast <unsigned> (time(0)));

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(1740, 900, "Networking Project #3 - Veronika Kotckovich, Jenny Moon, Ryan O'Donnell", NULL, NULL);
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
	double elapsedTime = 0.0f;
	double elapsedTime2 = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		if (playerCards.size() == 10)
		{
			for (int i = 0; i < 10; i++)
			{
				g_modelsToDraw[i]->vecTextures[0].name = textureNames[playerCards[i]];
			}
		}
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

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		const float MAX_DELTA_TIME = 0.1f;

		if (deltaTime > MAX_DELTA_TIME)
		{
			deltaTime = MAX_DELTA_TIME;
		}

		elapsedTime += deltaTime;
		if (elapsedTime > 0.6) //60Hz
		{
			messageSendProtocol->createBuffer(8);
			messageSendProtocol->messageHeader.command_id = 01;
			messageSendProtocol->messageBody.message = "GetUpdate";
			messageSendProtocol->sendMessage(*messageSendProtocol->buffer);

			std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
			send(Connection, &packet[0], packet.size(), 0);
		}

		if (PRESS) {
			messageSendProtocol->createBuffer(8);
			messageSendProtocol->sendID(*messageSendProtocol->buffer, activeId, 03); //sending id 25
			std::vector<char> packet = messageSendProtocol->buffer->mBuffer;

			send(Connection, &packet[0], packet.size(), 0);
			PRESS = !PRESS;
		}

		if (moveTheCard)
		{
			if (glm::distance(findObjectByUniqueID(cardToMove)->position, newPosition) < 4.0f)
			{
				moveTheCard = false;
				elapsedTime2 = 0.0;
				newPosition = glm::vec3(0.0f);
				cardToMove = -1;
			} else if (glm::distance(findObjectByUniqueID(cardToMove)->position, newPosition) > 95.0f)
			{
				moveTheCard = false;
				elapsedTime2 = 0.0;
				newPosition = glm::vec3(0.0f);
				cardToMove = -1;
			}
			else {
				updateCardPosition(elapsedTime2, cardToMove, newPosition);
				elapsedTime2 += deltaTime;
			}
		}


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
	//std::stringstream ssTitle;
	//ssTitle << activeLight->name << " - "<<activeLight->atten.x << " , "
	//	<< activeLight->atten.y << " , "
	//	<< activeLight->atten.z;

	//glfwSetWindowTitle(window, ssTitle.str().c_str());
}

void lightDebugSpheres(int program)
{
	if (pLightManager->vecLights.size() != 0)
	{
		cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSph");
		pDebugSphere->bIsVisiable = false;
		pDebugSphere->setDiffuseColor(glm::vec3(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f));
		pDebugSphere->bUseVertexColour = false;
		pDebugSphere->setUniformScale(1.0f);
		for (int i = 0; i < pLightManager->vecLights.size(); i++)
		{

			//pDebugSphere->position = pLightManager->vecLights[i]->position;
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

void playCard(int cardId)
{
	MessageProtocol* messageSendProtocol = new MessageProtocol();
	messageSendProtocol->createBuffer(8);
	messageSendProtocol->messageHeader.command_id = commandID;

	messageSendProtocol->sendCard(*messageSendProtocol->buffer, playerCards[cardId], cardId);

	std::vector<char> packet = messageSendProtocol->buffer->mBuffer;
	send(Connection, &packet[0], packet.size(), 0);
}

void updateCardPosition(double deltaTime, int id, glm::vec3 newPosition)
{
	float speed = 10.0f;
	glm::vec3 position = findObjectByUniqueID(id)->position;
	glm::vec3 initPosition = findObjectByUniqueID(id)->initialPosition;

	//std::cout << "newPosition " << newPosition.x << " " <<
	//	newPosition.y << " " <<
	//	newPosition.z << " " << std::endl;

	//std::cout << "position " << findObjectByUniqueID(id)->position.x << " " <<
	//	findObjectByUniqueID(id)->position.y << " " <<
	//	findObjectByUniqueID(id)->position.z << " " << std::endl;

	glm::vec3 direction = newPosition - initPosition;
	direction = glm::normalize(direction);
	//glm::vec3 tempPos = glm::smoothstep(initPosition, newPosition, position);
	float step = glm::smoothstep(1.0f, 0.0f, (float)deltaTime);

	//std::cout << "step " << step << std::endl;

	findObjectByUniqueID(id)->position.x += direction.x /** step*/*speed * deltaTime;
	findObjectByUniqueID(id)->position.y += direction.y /** step*/*speed * deltaTime;
	findObjectByUniqueID(id)->position.z += direction.z /** step*/*speed * deltaTime;
}