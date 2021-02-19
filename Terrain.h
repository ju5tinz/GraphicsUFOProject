#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "PerlinNoise.h"
#include "shader.h"
#include "Camera.h"

#include <vector>
#include <stdlib.h>
#include <time.h>  
#include <math.h>

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

#include "stb_image.h"

class Terrain {
private:
	int size;
	std::vector<std::vector<float>> heights;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCord;
	std::vector<GLuint> indices;

	GLuint vao, vbo[3], ebo;
	GLuint shaderProg;

	unsigned int texture;

	Camera* camera;
public:
	Terrain(int size, GLuint shaderProg);
	void generateHeights();
	void genTriangles();
	void init();
	void setCamera(Camera* camera) { this->camera = camera; };
	void draw();
};

#endif