

#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "camera.h"
#include "shader.h"
#include "texture.h"

#include <string>
#include <vector>
#include <unordered_map>

class Renderer {
public:
	GLuint width, height;

	GLboolean bloom;
	GLfloat exposure;

	GLuint hdrFBO;

	Renderer( int w, int h, Camera* cam );
	~Renderer();

	void drawFloor();

	void drawCubes( std::vector<glm::vec3> cubePositions );

	void drawText( std::string text, int pen_x, int pen_y );

	void drawLights( float waver );

private:
	GLfloat tree_glow[3] = { 0.9f, 0.9f, 0.9f };

	Camera* camera;

	GLuint cubeVAO;
	GLuint cubeVBO;

	GLuint quadVAO;
	GLuint quadVBO;

	GLuint colorBuffers[2];

	GLuint pingpongFBO[2];
    GLuint pingpongColorbuffers[2];

	std::unordered_map<std::string, Shader> shads;
	std::unordered_map<std::string, GLuint> textures;

	std::vector<glm::vec3> rand_positions;

	std::vector<glm::vec3> slow_po;

	std::vector<glm::vec3> rand_lights;

	FT_Library ft;
	FT_Face face;

	void my_draw_bitmap( FT_Bitmap* bm, int top_left_x, int top_left_y );
	
	void renderCube();
	void renderQuad();
};


#endif


