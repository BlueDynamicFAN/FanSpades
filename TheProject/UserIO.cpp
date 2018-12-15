//UserIO.cpp
//
//purpouse: Describes user's keybard commands
#include "global.h"
#include <iostream>
#include "cMeshObject.h"
#include "cLightManager.h"

extern unsigned int activeId;
unsigned int activeLightId = 0;
cMeshObject* activeModel = NULL;//findObjectByUniqueID(activeId);
extern cLightManager* pLightManager;
extern cLight* activeLight;
void playCard(int cardId);
extern bool PRESS;
extern bool isCardPlayed;
extern bool isGameOver;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	activeModel = g_modelsToDraw[11];

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		if (activeId == 0) {
			activeId = 10;
			activeId -= 1;
		}
		else {
			activeId -= 1;
		}

		activeModel->position = findObjectByUniqueID(activeId)->position;
		activeModel->position += glm::vec3(1.0f, -1.0, 1.0);

		std::cout << "Current selected card: " << findObjectByUniqueID(activeId)->friendlyName << std::endl;
	} 

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!isGameOver)
		{
			if (!isCardPlayed)
			{
				if (findObjectByUniqueID(activeId)->bIsVisiable)
				{
					//std::cout << "Card played: " << findObjectByUniqueID(activeId)->friendlyName << std::endl;
					playCard(activeId);
					//findObjectByUniqueID(activeId)->bIsVisiable = false;
					PRESS = true;
				}
				else
				{
					std::cout << "Card already played" << std::endl;
				}
			}
			else
			{
				std::cout << "Waiting for other player to play card" << std::endl;
			}
		}
		else
		{
			std::cout << "Game is over" << std::endl;
		}

	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		unsigned int num = 10;
		if (activeId >= num - 1) {
			activeId = 0;
		}
		else {
			activeId += 1;
		}

		activeModel->position = findObjectByUniqueID(activeId)->position;
		activeModel->position += glm::vec3(1.0f, -0.7, 0.4);

		std::cout << "Current selected card: " << findObjectByUniqueID(activeId)->friendlyName << std::endl;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		if (activeLightId == 0) {
			activeLightId = (unsigned int)pLightManager->vecLights.size();
			activeLightId -= 1;
		}
		else {
			activeLightId -= 1;
		}

		activeLight = pLightManager->vecLights[activeLightId];
		std::cout << activeLightId << " - " << activeLight->position.x << std::endl;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		unsigned int num = (unsigned int)pLightManager->vecLights.size();
		if (activeLightId >= num - 1) {
			activeLightId = 0;
		}
		else {
			activeLightId += 1;
		}

		activeLight = pLightManager->vecLights[activeLightId];
		std::cout << activeLightId << " - " << activeLight->position.x << std::endl;
	}

	return;
}

bool IsShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool IsCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool IsAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool AreAllModifiersUp(GLFWwindow* window)
{
	if (IsShiftDown(window)) { return false; }
	if (IsCtrlDown(window)) { return false; }
	if (IsAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

void processKeys(GLFWwindow* window)
{
	const float CAMERA_SPEED_SLOW = 0.7f;
	const float CAMERA_SPEED_FAST = 2.0f;
	const float MODEL_MOVE = 0.1f;
	const float MODEL_MOVE_FAST = 1.0f;
	float cameraSpeed = CAMERA_SPEED_SLOW;
	float modelSpeed = MODEL_MOVE;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		cameraSpeed = CAMERA_SPEED_FAST;
		modelSpeed = MODEL_MOVE_FAST;
	}

	if (!IsCtrlDown(window) && !IsAltDown(window) && !IsShiftDown(window)) {
		// ****  Z  ****
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //Forward
		{
			//checked every frame
			//g_FlyCamera->eye.z += cameraSpeed; //Clear the screen - Draw the bunny - check for key = over and over again
			g_FlyCamera->MoveForward_Z(cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //Bakward
		{
			//g_CameraEye.z -= cameraSpeed;
			g_FlyCamera->MoveForward_Z(-cameraSpeed);
		}

		// ****  X  ****
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //Right
		{
			/*g_CameraEye.x += cameraSpeed;*/
			g_FlyCamera->MoveLeftRight_X(-cameraSpeed);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //Left
		{
			g_FlyCamera->MoveLeftRight_X(+cameraSpeed);
		}

		// ****  Y  ****
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)//Up
		{

			::g_FlyCamera->MoveUpDown_Y(+cameraSpeed);

		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //Down
		{
			::g_FlyCamera->MoveUpDown_Y(-cameraSpeed);
		}
	}

	const float MIN_LIGHT_BRIGHTNESS = 0.001f;
	const float LIN_ATTEN_STEP = 0.001;
	const float QUAD_ATTEN_STEP = 0.00001;
	// Control (ctrl)  Move light


	//SAVING TO JSON
	if (IsAltDown(window) && IsCtrlDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_S)) 
		{
			loadALLEntitiesToJASON("./assets/JSON/entities2.json");
			pLightManager->saveLights("./assets/JSON/lights1.json");
			g_FlyCamera->saveParams("./assets/JSON/camera.json");
			
			
		}

		if (glfwGetKey(window, GLFW_KEY_M))
		{
			loadALLEntitiesToJASON("./assets/JSON/entities2.json");
			
		}

		if (glfwGetKey(window, GLFW_KEY_L)) 
		{
			pLightManager->saveLights("./assets/JSON/lights1.json");
			
		}

		if (glfwGetKey(window, GLFW_KEY_C))
		{
			g_FlyCamera->saveParams("./assets/JSON/camera.json");
			
		}
	}
	if (IsCtrlDown(window)&&!IsAltDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W)) { activeLight->position.z += modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_S)) { activeLight->position.z -= modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_A)) { activeLight->position.x -= modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_D)) { activeLight->position.x += modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_Q)) { activeLight->position.y += modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_E)) { activeLight->position.y -= modelSpeed; }

		if (glfwGetKey(window, GLFW_KEY_1)) { activeLight->atten.y += LIN_ATTEN_STEP; }
		if (glfwGetKey(window, GLFW_KEY_2)) { activeLight->atten.y -= LIN_ATTEN_STEP; }
		if (glfwGetKey(window, GLFW_KEY_3)) { activeLight->atten.z += QUAD_ATTEN_STEP; }
		if (glfwGetKey(window, GLFW_KEY_4)) { activeLight->atten.z -= QUAD_ATTEN_STEP; }
		if (glfwGetKey(window, GLFW_KEY_0)) { activeLight->setLightOn(false); }
		if (glfwGetKey(window, GLFW_KEY_9)) { activeLight->setLightOn(true); }
		if (glfwGetKey(window, GLFW_KEY_L)) 
		{ 
			float r = (float)rand() / RAND_MAX;
			float g = (float)rand() / RAND_MAX;
			float b = (float)rand() / RAND_MAX;
			activeLight->setDiffuse(glm::vec4(r,g,b,1.0));
		}
		if (glfwGetKey(window, GLFW_KEY_K)) { activeLight->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); }
		
		//DIRECTION
		const float ROTATE = 0.05;
		if (glfwGetKey(window, GLFW_KEY_X) && glfwGetKey(window, GLFW_KEY_EQUAL)) { activeLight->direction.x += ROTATE; }
		if (glfwGetKey(window, GLFW_KEY_X) && glfwGetKey(window, GLFW_KEY_MINUS)) { activeLight->direction.x -= ROTATE; }

		if (glfwGetKey(window, GLFW_KEY_Y) && glfwGetKey(window, GLFW_KEY_EQUAL)) { activeLight->direction.y += ROTATE; }
		if (glfwGetKey(window, GLFW_KEY_Y) && glfwGetKey(window, GLFW_KEY_MINUS)) { activeLight->direction.y -= ROTATE; }

		if (glfwGetKey(window, GLFW_KEY_Z) && glfwGetKey(window, GLFW_KEY_EQUAL)) { activeLight->direction.z += ROTATE; }
		if (glfwGetKey(window, GLFW_KEY_Z) && glfwGetKey(window, GLFW_KEY_MINUS)) { activeLight->direction.z -= ROTATE; }

	}//if(!IsShiftDown(window) )

	const float OBJ_SPEC = 1.0;
	const float ROTATE = 0.05;
	const float SCALE = 0.01;
	if (IsAltDown(window)&&!IsCtrlDown(window))
	{
		//CHANGING POSITION
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { activeModel->position.z += modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_S)) { activeModel->position.z -= modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_A)) { activeModel->position.x -= modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_D)) { activeModel->position.x += modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_Q)) { activeModel->position.y += modelSpeed; }
		if (glfwGetKey(window, GLFW_KEY_E)) { activeModel->position.y -= modelSpeed; }
		//CHANGING SPECULAR
		if (glfwGetKey(window, GLFW_KEY_C))
		{
			activeModel->materialSpecular.a += OBJ_SPEC;
			std::cout << "Specular is: " << activeModel->materialSpecular.a << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_V))
		{
			activeModel->materialSpecular.a -= OBJ_SPEC;
			std::cout << "Specular is: " << activeModel->materialSpecular.a << std::endl;
		}

		//CHANGING ROTATION
		
		if (glfwGetKey(window, GLFW_KEY_X) && glfwGetKey(window, GLFW_KEY_9)){ activeModel->adjMeshOrientationQ(glm::quat(glm::vec3(ROTATE, 0.0f, 0.0f)));}
		if (glfwGetKey(window, GLFW_KEY_X) && glfwGetKey(window, GLFW_KEY_0)) { activeModel->adjMeshOrientationQ(glm::quat(glm::vec3(-ROTATE, 0.0f, 0.0f))); }

		if (glfwGetKey(window, GLFW_KEY_Y) && glfwGetKey(window, GLFW_KEY_9)) { activeModel->adjMeshOrientationQ(glm::quat(glm::vec3(0.0f, ROTATE, 0.0f))); }
		if (glfwGetKey(window, GLFW_KEY_Y) && glfwGetKey(window, GLFW_KEY_0)) { activeModel->adjMeshOrientationQ(glm::quat(glm::vec3(0.0f, -ROTATE, 0.0f)));}

		if (glfwGetKey(window, GLFW_KEY_Z) && glfwGetKey(window, GLFW_KEY_9)) { activeModel->adjMeshOrientationQ(glm::quat(glm::vec3(0.0f, 0.0f, ROTATE))); }
		if (glfwGetKey(window, GLFW_KEY_Z) && glfwGetKey(window, GLFW_KEY_0)) { activeModel->adjMeshOrientationQ(glm::quat(glm::vec3(0.0f, 0.0f, -ROTATE)));}
		
		//CHANGING isVisiable
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) { activeModel->bIsVisiable = !activeModel->bIsVisiable;;
			Sleep(1);
		}
		//CHANGING isWireFrame
		bool temp = activeModel->bIsVisiable;
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			activeModel->bIsWireFrame = !activeModel->bIsWireFrame;
			Sleep(1);
		}

		//CHANGING SCALE
		if (glfwGetKey(window, GLFW_KEY_EQUAL)) { activeModel->setUniformScale(activeModel->nonUniformScale.x + SCALE); }
		if (glfwGetKey(window, GLFW_KEY_MINUS)) { activeModel->setUniformScale(activeModel->nonUniformScale.x - SCALE); }

	}//if(!IsAltDown(window) )

	if (IsShiftDown(window) && !IsAltDown(window) && !IsCtrlDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { ::g_FlyCamera->Pitch_UpDown(.1); }
		if (glfwGetKey(window, GLFW_KEY_E)) { ::g_FlyCamera->Pitch_UpDown(-.1); }
		if (glfwGetKey(window, GLFW_KEY_A)) { ::g_FlyCamera->Yaw_LeftRight(.1); }
		if (glfwGetKey(window, GLFW_KEY_D)) { ::g_FlyCamera->Yaw_LeftRight(-.1); }
		if (glfwGetKey(window, GLFW_KEY_W)) { ::g_FlyCamera->Roll_ClockWise(.1); }
		if (glfwGetKey(window, GLFW_KEY_S)) { ::g_FlyCamera->Roll_ClockWise(-.1); }

	}

	return;

}
