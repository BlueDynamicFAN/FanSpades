//PsysicsStuff.cpp
#include "global.h"
#include <glm/glm.hpp>
//#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include "sDrawInfo.h"
#include <algorithm>
#include <iostream>
#include <vector>

const float GROUND_PLANE_Y = -3.0f;

const float LIMIT_POS_X = 25.0f;
const float LIMIT_NEG_X = -25.0f;
const float LIMIT_POS_Z = 25.0f;
const float LIMIT_NEG_Z = -25.0f;
const float LIMIT_POS_Y = 30.0f;

typedef glm::vec3 Point;
typedef glm::vec3 Vector;

glm::vec3 thePoint;
struct Offset
{
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
};

void CalculateClosestSinglePointOnMeshWithOffset(sDrawInfo theMeshDrawInfo, glm::mat4x4 matModel, glm::vec3 pointToTest, glm::vec3 &closestPointToModel);
void CalculateClosestSinglePointOnMesh(sDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, glm::vec3 &closestPointToModel);
void CalculateClosestSinglePointOnMeshWithOffsetNormals(sDrawInfo theMeshDrawInfo, glm::mat4x4 matModel, glm::vec3 pointToTest, glm::vec3 &closestPointToModel, glm::vec3 &normal);

void getMatModel(glm::mat4x4 &matModel, cMeshObject* theMeshObj) {
	matModel = glm::mat4x4(1.0f);		/// mat4x4_identity(m);

	///Positioning model in the scene
	glm::mat4 matMove = glm::translate(glm::mat4(1.0f),
		theMeshObj->position);

	matModel = matModel * matMove;


	glm::quat qRoation = theMeshObj->getQOrientation();
	glm::mat4 matQrotate = glm::mat4(qRoation);

	matModel = matModel * matQrotate;

	/// And now scale

	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		theMeshObj->nonUniformScale);
	matModel = matModel * matScale;
}

bool isSphereColideTiltedRect(cMeshObject* sphEntity, cMeshObject* rectEntity, double deltaTime) {

	glm::vec3 oldPosition = sphEntity->position;
	sDrawInfo meshInfo;
	meshInfo.meshName = rectEntity->meshName;
	meshManager->FindDrawInfoByModelName(meshInfo);

	const float EPSILON = 0.3f;
	glm::vec3 closestPoint;
	glm::vec3 differenceOfOrigins = rectEntity->position - glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vecRealClosestPoint;
	glm::vec3 normal;
	glm::mat4x4 matModel;
	getMatModel(matModel, rectEntity);

	CalculateClosestSinglePointOnMeshWithOffsetNormals(meshInfo, matModel, sphEntity->position, closestPoint, normal);

	vecRealClosestPoint = closestPoint + differenceOfOrigins;

	if ((glm::length(sphEntity->position - closestPoint)) <= EPSILON + sphEntity->theParams->getSize().x)
	{
		//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
		sphEntity->collision->positionWhenCollide = oldPosition;
		rectEntity->materialDiffuse.r -= 0.01;
		rectEntity->materialDiffuse.g -= 0.01;
		if (normal.x > 0)
		{
			sphEntity->theParams->velocity.x = fabs(sphEntity->theParams->velocity.x);
			
		} else if (normal.x < 0)
		{
			sphEntity->theParams->velocity.x = -fabs(sphEntity->theParams->velocity.x);
		
		}
		if (normal.y > 0)
		{
			sphEntity->theParams->velocity.y = fabs(sphEntity->theParams->velocity.y);
			
		}else if (normal.y < 0)
		{
			sphEntity->theParams->velocity.y = -fabs(sphEntity->theParams->velocity.y);
			
		}
		if (normal.z < 0)
		{
			sphEntity->theParams->velocity.z = -fabs(sphEntity->theParams->velocity.z);
			
		}else if (normal.z > 0)
		{
			sphEntity->theParams->velocity.z = fabs(sphEntity->theParams->velocity.z);
	
		}

		//sphEntity->materialDiffuse = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

		sphEntity->position.x = sphEntity->collision->positionWhenCollide.x + sphEntity->theParams->velocity.x * deltaTime;
		sphEntity->position.y = sphEntity->collision->positionWhenCollide.y + sphEntity->theParams->velocity.y * deltaTime;
		sphEntity->position.z = sphEntity->collision->positionWhenCollide.z + sphEntity->theParams->velocity.z * deltaTime;

		return true;
	}

}

bool isSphereColideRect(cMeshObject* sphEntity, cMeshObject* rectEntity, double deltaTime) {

	
	glm::vec3 oldPosition = sphEntity->position;

	sDrawInfo meshInfo;
	meshInfo.meshName = rectEntity->meshName;
	meshManager->FindDrawInfoByModelName(meshInfo);

	const float EPSILON = 0.3f;
	glm::vec3 closestPoint;
	glm::vec3 differenceOfOrigins = rectEntity->position - glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vecRealClosestPoint;
	glm::vec3 normal;
	glm::mat4x4 matModel;
	getMatModel(matModel, rectEntity);

	CalculateClosestSinglePointOnMeshWithOffsetNormals(meshInfo, matModel, sphEntity->position, closestPoint, normal);

	vecRealClosestPoint = closestPoint + differenceOfOrigins;

	if ((glm::length(sphEntity->position - closestPoint)) <= EPSILON+ sphEntity->theParams->getSize().x)
	{
		sphEntity->collision->positionWhenCollide = oldPosition;
		rectEntity->materialDiffuse.r -= 0.01;
		rectEntity->materialDiffuse.b -= 0.01;
		if (normal.x > 0)
		{
			sphEntity->theParams->velocity.x = fabs(sphEntity->theParams->velocity.x);
		}
		else if (normal.x < 0)
		{
			sphEntity->theParams->velocity.x = -fabs(sphEntity->theParams->velocity.x);
		}
		else if (normal.y > 0)
		{
			sphEntity->theParams->velocity.y = fabs(sphEntity->theParams->velocity.y);
		}
		else if (normal.y < 0)
		{
			sphEntity->theParams->velocity.y = -fabs(sphEntity->theParams->velocity.y);
		}
		else if (normal.z < 0)
		{
			sphEntity->theParams->velocity.z = -fabs(sphEntity->theParams->velocity.z);
		}
		else if (normal.z > 0)
		{
			sphEntity->theParams->velocity.z = fabs(sphEntity->theParams->velocity.z);
		}

		sphEntity->position.x = sphEntity->collision->positionWhenCollide.x + sphEntity->theParams->velocity.x * deltaTime;
		sphEntity->position.y = sphEntity->collision->positionWhenCollide.y + sphEntity->theParams->velocity.y * deltaTime;
		sphEntity->position.z = sphEntity->collision->positionWhenCollide.z + sphEntity->theParams->velocity.z * deltaTime;

		return true;
	}
	return false;
}

bool isSphereTouchedTheGround(cMeshObject* sphEntity, cMeshObject* groundEntity) {


	glm::vec3 oldPosition = sphEntity->position;
	sDrawInfo meshInfo;
	meshInfo.meshName = groundEntity->meshName;
	meshManager->FindDrawInfoByModelName(meshInfo);

	const float EPSILON = 0.1f;
	glm::vec3 closestPoint;

	CalculateClosestSinglePointOnMesh(meshInfo, sphEntity->position, closestPoint);

	if ((glm::length(sphEntity->position - closestPoint)) <= EPSILON + sphEntity->theParams->getSize().x)
	{
		sphEntity->collision->positionWhenCollide = oldPosition;
		return true;
	}
	return false;
}

bool areSpheresPenetrating(cMeshObject* pA, cMeshObject* pB)
{
	glm::vec3 positionA = pA->position;
	glm::vec3 positionB = pB->position;

	if (glm::distance(pA->position, pB->position) <= (pA->theParams->getSize().x + pB->theParams->getSize().x))
	{
		pA->collision->positionWhenCollide = positionA;
		pA->collision->colideWithName = pB->friendlyName;
		pB->collision->positionWhenCollide = positionB;
		pB->collision->colideWithName = pA->friendlyName;
		return true;
	}
	return false;
}

bool updateSphereLocation(cMeshObject* sphEntity, glm::vec3 direction, double deltaTime)
{
	if (direction.x > 0)
	{
		sphEntity->theParams->velocity.x = fabs(sphEntity->theParams->velocity.x);

	}
	else if (direction.x < 0)
	{
		sphEntity->theParams->velocity.x = -fabs(sphEntity->theParams->velocity.x);

	}
	if (direction.y > 0)
	{
		sphEntity->theParams->velocity.y = fabs(sphEntity->theParams->velocity.y);

	}
	else if (direction.y < 0)
	{
		sphEntity->theParams->velocity.y = -fabs(sphEntity->theParams->velocity.y);

	}
	if (direction.z < 0)
	{
		sphEntity->theParams->velocity.z = -fabs(sphEntity->theParams->velocity.z);

	}
	else if (direction.z > 0)
	{
		sphEntity->theParams->velocity.z = fabs(sphEntity->theParams->velocity.z);

	}

	sphEntity->position.x = sphEntity->collision->positionWhenCollide.x + sphEntity->theParams->velocity.x * deltaTime;
	sphEntity->position.y = sphEntity->collision->positionWhenCollide.y + sphEntity->theParams->velocity.y * deltaTime;
	sphEntity->position.z = sphEntity->collision->positionWhenCollide.z + sphEntity->theParams->velocity.z * deltaTime;

	return true;
}


//Called every frame
void DoPhysicsUpdate(double deltaTime, std::vector<cMeshObject*> entities)
{
	const double LARGEST_DELTATIME = 0.10; //10 ms = 10Hz
	if (deltaTime > LARGEST_DELTATIME) {
		deltaTime = LARGEST_DELTATIME;
	}

	for (std::vector<cMeshObject*>::iterator itMesh = entities.begin();
		itMesh != entities.end(); itMesh++)
	{
		cMeshObject*  pCurMesh = *itMesh;
		cMeshObject* ground = findObjectByFriendlyName("Plane");
		
		if (pCurMesh->theParams->doesPhysincs && pCurMesh->theParams->theShape == shape::SPHERE)
		{
			//Euler integration
			float delatX;

			pCurMesh->theParams->velocity.x += pCurMesh->theParams->accel.x * deltaTime;
			pCurMesh->theParams->velocity.y += pCurMesh->theParams->accel.y * deltaTime;
			pCurMesh->theParams->velocity.z += pCurMesh->theParams->accel.z * deltaTime;

			pCurMesh->position.x += pCurMesh->theParams->velocity.x * deltaTime;
			pCurMesh->position.y += pCurMesh->theParams->velocity.y * deltaTime;
			pCurMesh->position.z += pCurMesh->theParams->velocity.z * deltaTime;

			//***SPHERE TO GROUND PLANE COLLISION DETECTION AND REACTION***
			/*if (isSphereTouchedTheGround(pCurEntity, ground))
			{
				pCurEntity->theParams->velocity *= 0.99;
				pCurEntity->theParams->velocity.y = fabs(pCurEntity->theParams->velocity.y);
				pCurEntity->position.x = pCurEntity->collision->positionWhenCollide.x + pCurEntity->theParams->velocity.x * deltaTime;
				pCurEntity->position.y = pCurEntity->collision->positionWhenCollide.y + pCurEntity->theParams->velocity.y * deltaTime;
				pCurEntity->position.z = pCurEntity->collision->positionWhenCollide.z + pCurEntity->theParams->velocity.z * deltaTime;
			}*/

			
			//***SPHERE TO BOXES DETECTION AND REACTION***

			

			

			//***SPHERE TO PLANES DETECTION AND REACTION***

			if (pCurMesh->position.x >= LIMIT_POS_X)
			{
				pCurMesh->theParams->velocity.x = -fabs(pCurMesh->theParams->velocity.x);

			}

			if (pCurMesh->position.x <= LIMIT_NEG_X)
			{
				pCurMesh->theParams->velocity.x = fabs(pCurMesh->theParams->velocity.x);

			}

			if (pCurMesh->position.z >= LIMIT_POS_Z)
			{
				pCurMesh->theParams->velocity.z = -fabs(pCurMesh->theParams->velocity.x);

			}

			if (pCurMesh->position.z <= LIMIT_NEG_Z)
			{
				pCurMesh->theParams->velocity.z = fabs(pCurMesh->theParams->velocity.z);

			}
		}
	}


	return;
}

bool isACloserThanB(glm::vec3 &A, glm::vec3 &B) {

	if (glm::length(thePoint - A) <= glm::length(thePoint - B))
	{
		return true;
	}

	return false;
}

Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
{
	Vector ab = b - a;  //one edge
	Vector ac = c - a;
	Vector bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	float snom = glm::dot(p - a, ab);
	float sdenom = glm::dot(p - b, a - b);
	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	float tnom = glm::dot(p - a, ac);
	float tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	float unom = glm::dot(p - b, bc);
	float udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
	Vector n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));
	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	float va = glm::dot(n, glm::cross(b - p, c - p));
	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	float vb = glm::dot(n, glm::cross(c - p, a - p));
	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}


void CalculateClosestPointOnMesh(sDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, std::vector<glm::vec3> &vecPoints)
{
	vecPoints.clear();

	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{
		//sPlyTriangle theTriangle;
		sPlyTriangle CurTri = theMeshDrawInfo.trianglesFromFile[triIndex];
		
		sPlyVertex corner_1 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3];

		//convert this to glm
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);

		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);
		vecPoints.push_back(closestPoint);
	}


	return;
}

void CalculateClosestTrianglePointOnMesh(sDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, glm::vec3 &closestPointToModel, glm::vec3 theVertices[])
{
	std::vector<glm::vec3> vecPoints;
	vecPoints.clear();

	glm::vec3 vertices[3];

	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{
		//sPlyTriangle theTriangle;
		sPlyTriangle CurTri = theMeshDrawInfo.trianglesFromFile[triIndex];
		//...call the ClosestPointToTriangles
		//Need to to get the 3 vertices of the triangles
		sPlyVertex corner_1 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3];

		//convert this to glm
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);


		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);


		if (vecPoints.size() == 0) {
			vecPoints.push_back(closestPoint);
			theVertices[0].x = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1].x;
			theVertices[0].y = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1].y;
			theVertices[0].z = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1].z;
			theVertices[1].x = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2].x;
			theVertices[1].y = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2].y;
			theVertices[1].z = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2].z;
			theVertices[2].x = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3].x;
			theVertices[2].y = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3].y;
			theVertices[2].z = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3].z;
			///std::cout << vertices[2].x << " " << vertices[2].y << " " << vertices[2].z << std::endl;
			///std::cout << theVertices[2].x << " " << theVertices[2].y << " " << theVertices[2].z << std::endl;
		}
		else if (glm::length(pointToTest - vecPoints[0]) > glm::length(pointToTest - closestPoint)) {
			vecPoints[0] = closestPoint;
			theVertices[0].x = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1].x;
			theVertices[0].y = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1].y;
			theVertices[0].z = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1].z;
			theVertices[1].x = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2].x;
			theVertices[1].y = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2].y;
			theVertices[1].z = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2].z;
			theVertices[2].x = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3].x;
			theVertices[2].y = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3].y;
			theVertices[2].z = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3].z;
			///std::cout << theVertices[2].x << " " << theVertices[2].y << " " << theVertices[2].z << std::endl;
		}

	}

	//std::sort(vecPoints.begin(), vecPoints.end(), isACloserThanB); BAD performance 
	closestPointToModel = vecPoints[0];

	return;
}


void CalculateClosestSinglePointOnMesh(sDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, glm::vec3 &closestPointToModel)
{
	std::vector<glm::vec3> vecPoints;
	closestPointToModel = glm::vec3(0.0f);


	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{
		
		sPlyTriangle CurTri = theMeshDrawInfo.trianglesFromFile[triIndex];
		//...call the ClosestPointToTriangles
		//Need to to get the 3 vertices of the triangles
		sPlyVertex corner_1 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3];

		//convert this to glm
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);


		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);


		if (vecPoints.size() == 0) {
			vecPoints.push_back(closestPoint);
			///std::cout << vertices[2].x << " " << vertices[2].y << " " << vertices[2].z << std::endl;
			///std::cout << theVertices[2].x << " " << theVertices[2].y << " " << theVertices[2].z << std::endl;
		}
		else if (glm::length(pointToTest - vecPoints[0]) > glm::length(pointToTest - closestPoint)) {
			vecPoints[0] = closestPoint;
			///std::cout << theVertices[2].x << " " << theVertices[2].y << " " << theVertices[2].z << std::endl;
		}

	}

	closestPointToModel = vecPoints[0];

	return;
}


void CalculateClosestSinglePointOnMeshWithOffset(sDrawInfo theMeshDrawInfo, glm::mat4x4 matModel, glm::vec3 pointToTest, glm::vec3 &closestPointToModel)
{
	std::vector<glm::vec3> vecPoints;
	closestPointToModel = glm::vec3(0.0f);


	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{

		sPlyTriangle CurTri = theMeshDrawInfo.trianglesFromFile[triIndex];
		//...call the ClosestPointToTriangles
		//Need to to get the 3 vertices of the triangles
		sPlyVertex corner_1 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3];

		//convert this to glm
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z); 
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);

		glm::vec3 vAct[3];

		vAct[0] = matModel * glm::vec4(vert_1, 1);
		vAct[1] = matModel * glm::vec4(vert_2, 1);
		vAct[2] = matModel * glm::vec4(vert_3, 1);

		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vAct[0], vAct[1], vAct[2]);


		if (vecPoints.size() == 0) {
			vecPoints.push_back(closestPoint);
		}
		else if (glm::length(pointToTest - vecPoints[0]) > glm::length(pointToTest - closestPoint)) {
			vecPoints[0] = closestPoint;
		}

	}

	closestPointToModel = vecPoints[0];

	return;
}

void CalculateClosestSinglePointOnMeshWithOffsetNormals(sDrawInfo theMeshDrawInfo, glm::mat4x4 matModel, glm::vec3 pointToTest, glm::vec3 &closestPointToModel, glm::vec3 &normal)
{
	std::vector<glm::vec3> vecPoints;
	closestPointToModel = glm::vec3(0.0f);


	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{

		sPlyTriangle CurTri = theMeshDrawInfo.trianglesFromFile[triIndex];
		//...call the ClosestPointToTriangles
		//Need to to get the 3 vertices of the triangles
		sPlyVertex corner_1 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.verticesFromFile[CurTri.vertex_index_3];

		/*float noramlX = (corner_1.nx + corner_2.nx + corner_3.nx) / 3;
		float noramlY = (corner_1.ny + corner_2.ny + corner_3.ny) / 3;
		float noramlZ = (corner_1.nz + corner_2.nz + corner_3.nz) / 3;*/


		//normal = glm::vec3(noramlX, noramlY, noramlZ);

		//convert this to glm
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);

		glm::vec3 vAct[3];

		vAct[0] = matModel * glm::vec4(vert_1, 1);
		vAct[1] = matModel * glm::vec4(vert_2, 1);
		vAct[2] = matModel * glm::vec4(vert_3, 1);

		glm::vec3 closestPoint = ClosestPtPointTriangle(pointToTest, vAct[0], vAct[1], vAct[2]);

		glm::vec3 vecOne;
		glm::vec3 vecTwo;


		if (vecPoints.size() == 0) {
			vecPoints.push_back(closestPoint);
			vecOne = vAct[1] - vAct[0];
			vecTwo = vAct[2] - vAct[0];

			normal = glm::cross(vecOne, vecTwo);
		}
		else if (glm::length(pointToTest - vecPoints[0]) > glm::length(pointToTest - closestPoint)) {
			vecPoints[0] = closestPoint;

			vecOne = vAct[1] - vAct[0];
			vecTwo = vAct[2] - vAct[0];

			normal = glm::cross(vecOne, vecTwo);
		}

	}

	closestPointToModel = vecPoints[0];

	return;
}