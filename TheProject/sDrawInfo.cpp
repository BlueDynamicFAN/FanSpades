#include "sDrawInfo.h"

sDrawInfo::sDrawInfo()
{
	this->meshName = "";

	this->VAO_ID = 0;

	this->indexBufferID = 0;
	this->indexBufferStart = 0;
	this->numberOfIndices = 0;
	unsigned int numberOfTriangles = 0;

	this->vertexBufferID = 0;
	this->vertexBufferStart = 0;
	this->numberOfVertices = 0;


	this->verticesFromFile = NULL;
	this->trianglesFromFile = NULL;

	this->verticesShader = NULL;

	this->pIndices = NULL;
}