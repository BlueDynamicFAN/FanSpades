#ifndef Camera_HG
#define Camera_HG
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3

class Camera
{
public:
	~Camera();
	static Camera* getCamera();
	void loadParams(std::string filename);
	void saveParams(std::string filename);
	glm::vec3 eye;
private:
	Camera();
	static Camera* cam;
};
#endif // !Camera_HG

