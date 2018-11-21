#ifndef Param_HG
#define Param_HG
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3

enum shape {
	UNKNOWN = 0,
	SPHERE = 1,
	TRIAN = 2,
	CUBE = 3
};

class Param
{
public:
	Param();
	~Param();
	Param(shape s);
	Param(shape s, glm::vec3 size, glm::vec3 vel, glm::vec3 accel);
	void setSize(glm::vec3 size);
	void setVelocity(glm::vec3 vel);
	void setAccel(glm::vec3 accel);

	bool doesPhysincs;

	glm::vec3 getSize();
	glm::vec3 getVelocity(void);
	glm::vec3 getAccel(void);
	shape theShape;

	glm::vec3 velocity;
	glm::vec3 accel;
private:

	float m_radius;

	float m_width;
	float m_height;
	float m_depth;

	float m_side1;
	float m_side2;
	float m_side3;

};

#endif
