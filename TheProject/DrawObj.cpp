#include "global.h"
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

bool HACK_bTextureUniformLocationsLoaded = false;
GLint tex00_UniLoc = -1;
GLint tex01_UniLoc = -1;
GLint tex02_UniLoc = -1;
GLint tex03_UniLoc = -1;
GLint tex04_UniLoc = -1;
GLint tex05_UniLoc = -1;
GLint tex06_UniLoc = -1;
GLint tex07_UniLoc = -1;

GLint texBW_0_UniLoc = -1;
GLint texBW_1_UniLoc = -1;

// Will bind the textures in use for this object on this draw call
void BindTextures(cMeshObject* pCurrentMesh, GLuint shaderProgramID)
{

	// This is pretty much a hack, so we should likely pass the shader object 
	// (pointer) to this function, and to the DrawObject call, too. 
	// (Another option would be to pass the shader MANAGER instead, so 
	//  that the functions can look up various things in the shader)
	//
	// For now, I'm going to get the uniform location here 
	// (to make it clear (maybe?) that we'll NEED those shader uniform locations)

	// So this is only called once... 
	if (!HACK_bTextureUniformLocationsLoaded)
	{
		tex00_UniLoc = glGetUniformLocation(shaderProgramID, "texture00");		// uniform sampler2D texture00;
		tex01_UniLoc = glGetUniformLocation(shaderProgramID, "texture01");		// uniform sampler2D texture01;
		tex02_UniLoc = glGetUniformLocation(shaderProgramID, "texture02");		// uniform sampler2D texture02;
		tex03_UniLoc = glGetUniformLocation(shaderProgramID, "texture03");		// uniform sampler2D texture03;
		tex04_UniLoc = glGetUniformLocation(shaderProgramID, "texture04");		// uniform sampler2D texture04;
		tex05_UniLoc = glGetUniformLocation(shaderProgramID, "texture05");		// uniform sampler2D texture05;
		tex06_UniLoc = glGetUniformLocation(shaderProgramID, "texture06");		// uniform sampler2D texture06;
		tex07_UniLoc = glGetUniformLocation(shaderProgramID, "texture07");		// uniform sampler2D texture07;

		texBW_0_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[0]");	// uniform vec4 texBlendWeights[2];
		texBW_1_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[1]");	// uniform vec4 texBlendWeights[2];

		HACK_bTextureUniformLocationsLoaded = true;

	}//if(!HACK_bTextureUniformLocationsLoaded )

	// For each texture, bind the texture to a texture unit and sampler
	// Texture #0 (on the mesh) -- Texture Unit 0 -- Sampler 0
	// Texture #1 (on the mesh) -- Texture Unit 1 -- Sampler 1
	// ....

	// Set all the blend weights (strengths) to zero
	float blendWeights[8] = { 0 };


	for (int texBindIndex = 0; texBindIndex != pCurrentMesh->vecTextures.size(); texBindIndex++)
	{
		// Bind to the the "texBindIndex" texture unit
		glActiveTexture(GL_TEXTURE0 + texBindIndex);

		// Connect the specific texture to THIS texture unit
		std::string texName = pCurrentMesh->vecTextures[texBindIndex].name;

		GLuint texID = ::g_pTheTextureManager->getTextureIDFromName(texName);

		glBindTexture(GL_TEXTURE_2D, texID);

		// Use a switch to pick the texture sampler and weight (strength)
		// BECAUSE the samplers can't be in an array
		switch (texBindIndex)
		{
		case 0:		// uniform sampler2D texture00  AND texBlendWeights[0].x;
			glUniform1i(tex00_UniLoc, texBindIndex);
			break;
		case 1:		// uniform sampler2D texture01  AND texBlendWeights[0].y;
			glUniform1i(tex01_UniLoc, texBindIndex);
			break;
		case 2:
			glUniform1i(tex02_UniLoc, texBindIndex);
			break;
		case 3:
			glUniform1i(tex03_UniLoc, texBindIndex);
			break;
		case 4:		// uniform sampler2D texture04  AND texBlendWeights[1].x;
			glUniform1i(tex04_UniLoc, texBindIndex);
			break;
		case 5:
			glUniform1i(tex05_UniLoc, texBindIndex);
			break;
		case 6:
			glUniform1i(tex06_UniLoc, texBindIndex);
			break;
		case 7:
			glUniform1i(tex07_UniLoc, texBindIndex);
			break;
		}//switch ( texBindIndex )

		// Set the blend weight (strengty)
		blendWeights[texBindIndex] = pCurrentMesh->vecTextures[texBindIndex].strength;

	}//for ( int texBindIndex

	// Set the weights (strengths) in the shader
	glUniform4f(texBW_0_UniLoc, blendWeights[0], blendWeights[1], blendWeights[2], blendWeights[3]);
	glUniform4f(texBW_1_UniLoc, blendWeights[4], blendWeights[5], blendWeights[6], blendWeights[7]);

	return;
}

void DrawObj(cMeshObject* currentObj, int program, glm::mat4x4 matModel)
{
	if (!currentObj->bIsVisiable)
	{
		return;
	}

	// Set up the texture binding for this object
	BindTextures(currentObj, program);

	GLint objectDiffuse_UniLoc = glGetUniformLocation(program, "objectDiffuse");
	GLint objectSpecular_UniLoc = glGetUniformLocation(program, "objectSpecular");
	GLint bDontUseLight_UNILOC = glGetUniformLocation(program, "bDontLight");
	GLint useVertexColour_UniLoc = glGetUniformLocation(program, "useVertexColour");
	GLint matMoldel_location = glGetUniformLocation(program, "matModel");
	GLint matMoldelInvTrans_location = glGetUniformLocation(program, "matModelInvTrans");



	glm::mat4 matTranslation = glm::translate(IDENTITY,
		currentObj->position);

	matModel = matModel * matTranslation;

	glm::quat qRoation = currentObj->getQOrientation();
	glm::mat4 matQrotate = glm::mat4(qRoation);

	matModel = matModel * matQrotate;

	// Calculate the inverse transpose before the scaling
	glm::mat4 matModelInvTrans = glm::inverse(glm::transpose(matModel));

	glm::mat4 matScale = glm::scale(IDENTITY,
		currentObj->nonUniformScale);
	matModel = matModel * matScale;

	/*matModel = matModel * rotateZ;
	matModel = matModel * move;*/


	glUniformMatrix4fv(matMoldel_location, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(matMoldelInvTrans_location, 1, GL_FALSE, glm::value_ptr(matModelInvTrans));

	glUniform4f(objectDiffuse_UniLoc,
		currentObj->materialDiffuse.r,
		currentObj->materialDiffuse.g,
		currentObj->materialDiffuse.b,
		currentObj->materialDiffuse.a);

	glUniform4f(objectSpecular_UniLoc,
		currentObj->materialSpecular.r,
		currentObj->materialSpecular.g,
		currentObj->materialSpecular.b,
		currentObj->materialSpecular.a);

	//*********Alpha transperance***************

	glEnable(GL_BLEND);
	//Source - is already in the framebuffer
	//Dest - what you are about to draw
	//This is a regulat alpha blend transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLint wholeObjectAlphaTransperancy_LocID = glGetUniformLocation(program, "wholeObjectAlphaTransperancy");
	glUniform1f(wholeObjectAlphaTransperancy_LocID, currentObj->materialDiffuse.a);

	//*******************************************
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(program);


	if (currentObj->bUseVertexColour)
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_FALSE);
	}


	if (currentObj->noLight)
	{
		glUniform1f(bDontUseLight_UNILOC, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(bDontUseLight_UNILOC, (float)GL_FALSE);
	}


	if (currentObj->bIsWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	sDrawInfo modelInfo;
	modelInfo.meshName = currentObj->meshName;

	if (meshManager->FindDrawInfoByModelName(modelInfo))
	{
		//glDrawArrays(GL_TRIANGLES, 0, bunnyInfo.numberOfIndices );
		glBindVertexArray(modelInfo.VAO_ID);
		glDrawElements(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);

		glBindVertexArray(0);

	}

}