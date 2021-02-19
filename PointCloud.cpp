#include "PointCloud.h"
using namespace std;

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) : pointSize(pointSize){
    //parse file and fill points, normals, and indices vertex
    objFile.open(objFilename);
    if (objFile.is_open()){
        string line;
        while (std::getline(objFile, line))
        {
            std::stringstream ss;
            ss << line;
            std::string label;
            ss >> label;
            if (label == "v") {
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                points.push_back(point);
            }
            else if (label == "vn"){
                glm::vec3 vn;
                ss >> vn.x >> vn.y >> vn.z;
                normals.push_back(vn);
            }
            else if (label == "f"){
                string word1, word2, word3;
                glm::ivec3 face;
                ss >> word1 >> word2 >> word3;
                regex rgx("(\\d+).{2}(\\d+)");
                smatch match;
                if(regex_search(word1, match, rgx))
                    face.x = stoi(match.str(1)) - 1;
                if(regex_search(word2, match, rgx))
                    face.y = stoi(match.str(1)) - 1;
                if(regex_search(word3, match, rgx))
                    face.z = stoi(match.str(1)) - 1;
                indices.push_back(face);
            }
        }
    }
    else{
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    objFile.close();
    
    // Set the model matrix to an identity matrix.
    model = glm::mat4(1);
    
	// VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
    // VBOs
    glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3)*indices.size(), indices.data(), GL_STATIC_DRAW);
	// Unbind from the VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointCloud::~PointCloud() {
	// Delete the VBO and the VAO.
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);
}

void PointCloud::draw(unsigned int cubemapTexture) {
	glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, indices.size()*3.0f, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void PointCloud::update() {
}

void PointCloud::updatePointSize(GLfloat size) {
    if(!size){
        //input size==0 so want to decrement
        pointSize /= 2;
    }
    else{
        //else just increment size
        pointSize *= 2;
    }
}



void PointCloud::spin(float deg){
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 PointCloud::getModel(){
    return model;
}
