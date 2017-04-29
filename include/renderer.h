
#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "camera.h"

class Renderer {
public:
	GLuint width, height;
	Renderer( GLuint w, GLuint h, Camera* cam );
	~Renderer();
	void draw();
	void init();
private:
	Camera* camera;
	GLuint VAO;
	std::vector<glm::vec3> cubePositions;
	glm::vec3 lightPos;
};

#endif
