#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <regex>

#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>



class PointCloud
{
private:
    std::ifstream objFile; // The obj file we are reading.
    
	std::vector<glm::vec3> points; // vertices
    std::vector<glm::vec3> normals; // normals
    std::vector<glm::ivec3> indices; //face indices
        
	GLuint vao, vbo[2], ebo;
	GLfloat pointSize;
    glm::mat4 model;
    
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw(unsigned int cubemapTexture);

	void update();

    glm::mat4 getModel();
    
	void updatePointSize(GLfloat size);

	void spin(float deg);
    void scale(int dir);
    void rotate(float rotAngle, glm::vec3 rotAxis);
    
};

#endif

