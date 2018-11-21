#ifndef sDrawInfo_HG
#define sDrawInfo_HG
#include <string>

#include "sPlyVertex.h"
#include "sPlyTriangle.h"


struct vert_xyz_rgb_n_uv {
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float r = 0.0f, g = 0.0f, b = 0.0f;
	float nx = 0.0f, ny = 0.0f, nz = 0.0f;
	float u0 = 0.0f, v0 = 0.0f, u1 = 0.0f, v1 = 0.0f;

};

struct sDrawInfo 
{
	sDrawInfo();
	std::string meshName;

	unsigned int VAO_ID;

	unsigned int indexBufferID;
	unsigned int indexBufferStart;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	unsigned int vertexBufferID;
	unsigned int vertexBufferStart;
	unsigned int numberOfVertices;


	sPlyVertex* verticesFromFile;
	sPlyTriangle* trianglesFromFile;

	vert_xyz_rgb_n_uv* verticesShader;

	unsigned int* pIndices;
};

#endif
