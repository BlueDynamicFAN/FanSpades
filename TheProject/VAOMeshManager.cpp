#include "VAOMeshManager.h"
#include "global.h"
#include <fstream>

VAOMeshManager::VAOMeshManager()
{
	return;
}
VAOMeshManager::~VAOMeshManager()
{
	return;
}

bool VAOMeshManager::FindDrawInfoByModelName(sDrawInfo &drawInfo)
{
	std::map< std::string /*model name*/, sDrawInfo >::iterator itModel = this->m_mapModel_to_VAOID.find(drawInfo.meshName);

	if (itModel == this->m_mapModel_to_VAOID.end())
	{
		// Didn't find model
		return false;
	}

	// Copy the draw info back to the caller.
	drawInfo = itModel->second;

	return true;
}

bool VAOMeshManager::LoadModelIntoVAO(sDrawInfo &drawInfo,
	unsigned int shaderProgramID) {

	//Load the model from the file
	if (!this->m_LoadModelFromFile(drawInfo)) {
		std::cout << "Can't load model from the file" << std::endl;
		return false;
	}

	//Load into GPU
	if (!this->m_LoadDrawInfo_Into_VAO(drawInfo, shaderProgramID)) {
		std::cout << "Can't upload model to the VAO" << std::endl;
		return false;
	}

	//Store this mesh into the map
	this->m_mapModel_to_VAOID[drawInfo.meshName] = drawInfo;

	return true;

}

bool VAOMeshManager::m_LoadModelFromFile(sDrawInfo &drawInfo)
{
	std::string nextData;

	std::ifstream file(drawInfo.meshName.c_str());

	if (!file.is_open()) {
		std::cout << "Can't open a mesh file" << std::endl;
		return false;
	}

	while (file >> nextData) {
		if (nextData == "vertex") {
			break;
		}
	}
	file >> drawInfo.numberOfVertices;
	std::cout << "Numbers of vertices: " << drawInfo.numberOfVertices << std::endl;
	drawInfo.verticesFromFile = new sPlyVertex[drawInfo.numberOfVertices];

	while (file >> nextData) {
		if (nextData == "face") {
			break;
		}
	}
	file >> drawInfo.numberOfTriangles;
	std::cout << "Numbers of faces(triangles): " << drawInfo.numberOfTriangles << std::endl;
	drawInfo.trianglesFromFile = new sPlyTriangle[drawInfo.numberOfTriangles];

	while (file >> nextData) {
		if (nextData == "end_header") {
			break;
		}
	}

	memset(drawInfo.verticesFromFile, 0, sizeof(sPlyVertex) * drawInfo.numberOfVertices);

	for (unsigned int i = 0; i < drawInfo.numberOfVertices; i++) {
		file >> drawInfo.verticesFromFile[i].x;
		file >> drawInfo.verticesFromFile[i].y;
		file >> drawInfo.verticesFromFile[i].z;
		file >> drawInfo.verticesFromFile[i].nx;
		file >> drawInfo.verticesFromFile[i].ny;
		file >> drawInfo.verticesFromFile[i].nz;

		file >> drawInfo.verticesFromFile[i].u;
		file >> drawInfo.verticesFromFile[i].v;


	}

	memset(drawInfo.trianglesFromFile, 0, sizeof(sPlyTriangle) * drawInfo.numberOfTriangles);

	unsigned int junk = 0;
	for (unsigned int i = 0; i < drawInfo.numberOfTriangles; i++) {

		file >> junk;
		file >> drawInfo.trianglesFromFile[i].vertex_index_1;
		file >> drawInfo.trianglesFromFile[i].vertex_index_2;
		file >> drawInfo.trianglesFromFile[i].vertex_index_3;
	}

	std::cout << "Read model from file" << std::endl;
	return true;
}

bool VAOMeshManager::m_LoadDrawInfo_Into_VAO(sDrawInfo &drawInfo, int program)
{
	//Vertex buffer
	drawInfo.verticesShader = new vert_xyz_rgb_n_uv[drawInfo.numberOfVertices];

	for (unsigned int index = 0; index != drawInfo.numberOfVertices; index++) {

		drawInfo.verticesShader[index].x = drawInfo.verticesFromFile[index].x;
		drawInfo.verticesShader[index].y = drawInfo.verticesFromFile[index].y;
		drawInfo.verticesShader[index].z = drawInfo.verticesFromFile[index].z;
		drawInfo.verticesShader[index].nx = drawInfo.verticesFromFile[index].nx;
		drawInfo.verticesShader[index].ny = drawInfo.verticesFromFile[index].ny;
		drawInfo.verticesShader[index].nz = drawInfo.verticesFromFile[index].nz;
		
		drawInfo.verticesShader[index].u0 = drawInfo.verticesFromFile[index].u;
		drawInfo.verticesShader[index].v0 = drawInfo.verticesFromFile[index].v;

		drawInfo.verticesShader[index].u1 = 0.0f;
		drawInfo.verticesShader[index].v1 = 0.0f;

		
		drawInfo.verticesShader[index].r = 1.0f;
		drawInfo.verticesShader[index].g = 1.0f;
		drawInfo.verticesShader[index].b = 1.0f;
	}

	glGenVertexArrays(1, &(drawInfo.VAO_ID));
	glBindVertexArray(drawInfo.VAO_ID);

	glGenBuffers(1, &(drawInfo.vertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.vertexBufferID);

	unsigned int sizeOfVertexBuffer = sizeof(vert_xyz_rgb_n_uv) * drawInfo.numberOfVertices;
		glBufferData(GL_ARRAY_BUFFER, sizeOfVertexBuffer, drawInfo.verticesShader, GL_STATIC_DRAW);

	//Index buffer
	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
	drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];
	memset(drawInfo.pIndices, 0, sizeof(unsigned int) * drawInfo.numberOfIndices);
	
	unsigned int indexIndex = 0;
	for (unsigned int triIndex = 0; triIndex != drawInfo.numberOfTriangles;
		triIndex++, indexIndex += 3)
	{
		drawInfo.pIndices[indexIndex + 0] = drawInfo.trianglesFromFile[triIndex].vertex_index_1;
		drawInfo.pIndices[indexIndex + 1] = drawInfo.trianglesFromFile[triIndex].vertex_index_2;
		drawInfo.pIndices[indexIndex + 2] = drawInfo.trianglesFromFile[triIndex].vertex_index_3;
	}

	glGenBuffers(1, &(drawInfo.indexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.indexBufferID);

	unsigned int indexBufferSizeInBytes = sizeof(unsigned int) * drawInfo.numberOfIndices;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indexBufferSizeInBytes,	
		drawInfo.pIndices,
		GL_STATIC_DRAW);

	GLint vpos_location = glGetAttribLocation(program, "vPos");
	GLint vcol_location = glGetAttribLocation(program, "vCol");
	GLint norm_location = glGetAttribLocation(program, "vNorm");
	GLint vUVx2_location = glGetAttribLocation(program, "vUV_x2");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(vert_xyz_rgb_n_uv), (void*)0);

	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(vert_xyz_rgb_n_uv), (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(norm_location);
	glVertexAttribPointer(norm_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(vert_xyz_rgb_n_uv), (void*)(sizeof(float) * 6));

	glEnableVertexAttribArray(vUVx2_location);
	glVertexAttribPointer(vUVx2_location, 4, GL_FLOAT, GL_FALSE,
		sizeof(vert_xyz_rgb_n_uv), (void*)offsetof(vert_xyz_rgb_n_uv, u0));

	// Set the "current" VAO to nothing.
	glBindVertexArray(0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(norm_location);
	glDisableVertexAttribArray(vUVx2_location);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return true;

}