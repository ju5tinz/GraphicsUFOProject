#ifndef _FLYER_H_
#define _FLYER_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

class Flyer {
protected:
	glm::mat4 model;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;

	GLuint vao, vbo[2], ebo;
	GLuint shaderProg;

public:
	Flyer(std::string objFilename, GLuint shaderProgram, glm::mat4 model);

	void draw();
	void update(glm::mat4 transform);

	float x_loc, y_loc;
};

#endif
