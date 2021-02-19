#include "Flyer.h"

Flyer::Flyer(std::string objFilename, GLuint shaderProgram, glm::mat4 model) : 
	shaderProg(shaderProgram), model(model) {
	std::ifstream objFile(objFilename); // Open obj file

	// If the file has been opened
	if (objFile.is_open()) {
		std::vector<GLuint> vertex_indices;
		std::vector<GLuint> normal_indices;
		std::vector<glm::vec3> input_vertices;
		std::vector<glm::vec3> input_normals;

		std::string line;

		// Read lines from file
		while (std::getline(objFile, line)) {
			std::stringstream ss;
			ss << line;

			// Read first char of line
			std::string label;
			ss >> label;

			// If the the first char is v, then it is a vertex
			// Add the vertex to the points vector
			if (label == "v") {
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;
				input_vertices.push_back(point);
			}
			if (label == "vn") {
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;
				input_normals.push_back(normal);
			}
			if (label == "f") {
				/*
				std::string vec1str, vec2str, vec3str;
				std::getline(ss, vec1str, '/');
				vec1str.erase(0, 1);
				ss.ignore(256, ' ');
				std::getline(ss, vec2str, '/');
				ss.ignore(256, ' ');
				std::getline(ss, vec3str, '/');

				GLuint vec1, vec2, vec3;
				vec1 = std::stoul(vec1str);
				vec2 = std::stoul(vec2str);
				vec3 = std::stoul(vec3str);

				glm::ivec3 face;
				face.x = vec1 - 1;
				face.y = vec2 - 1;
				face.z = vec3 - 1;

				faces.push_back(face);
				*/

				std::string vertex1, vertex2, vertex3;
				GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
				
				//sscanf(ss.str(), "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				std::string token;
				std::getline(ss, token, '/');
				vertexIndex[0] = atoi(token.c_str());
				std::getline(ss, token, '/');
				std::getline(ss, token, ' ');
				normalIndex[0] = atoi(token.c_str());
				//std::getline(ss, token, ' ');
				std::getline(ss, token, '/');
				vertexIndex[1] = atoi(token.c_str());
				std::getline(ss, token, '/');
				std::getline(ss, token, ' ');
				normalIndex[1] = atoi(token.c_str());
				//std::getline(ss, token, ' ');
				std::getline(ss, token, '/');
				vertexIndex[2] = atoi(token.c_str());
				std::getline(ss, token, '/');
				std::getline(ss, token, ' ');
				normalIndex[2] = atoi(token.c_str());

				vertex_indices.push_back(vertexIndex[0]);
				vertex_indices.push_back(vertexIndex[1]);
				vertex_indices.push_back(vertexIndex[2]);

				normal_indices.push_back(normalIndex[0]);
				normal_indices.push_back(normalIndex[1]);
				normal_indices.push_back(normalIndex[2]);
			}
		}

		for (GLuint i = 0; i < vertex_indices.size(); i++) {
			points.push_back(input_vertices[vertex_indices[i]]);
			normals.push_back(input_normals[normal_indices[i]]);
			indices.push_back(i);
		}
	}
	// If the file cannot be opened
	else {
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}

	objFile.close(); //close file

	// set max and min points to the first point in points
	float max_x = points[0].x;
	float min_x = points[0].x;
	float max_y = points[0].y;
	float min_y = points[0].y;
	float max_z = points[0].z;
	float min_z = points[0].z;

	// find min and max values for x, y, z
	for (int i = 1; i < points.size(); i++) {
		glm::vec3 point = points[i];
		if (point.x > max_x)
			max_x = point.x;
		if (point.x < min_x)
			min_x = point.x;
		if (point.y > max_y)
			max_y = point.y;
		if (point.y < min_y)
			min_y = point.y;
		if (point.z > max_z)
			max_z = point.z;
		if (point.z < min_z)
			min_z = point.z;
	}

	// find center for x, y, z
	float center_x = (max_x + min_x) / 2;
	float center_y = (max_y + min_y) / 2;
	float center_z = (max_z + min_z) / 2;
	glm::vec3 center_vec = glm::vec3(0, 0, 0);

	//float max_dist = 0;

	for (int i = 0; i < points.size(); i++) {
		points[i].x -= center_x;
		points[i].y -= center_y;
		points[i].z -= center_z;

	}

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);
	glGenBuffers(1, &ebo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(),
		normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);


	// Bind to the ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the EBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Flyer::draw() {
	glUseProgram(shaderProg);
	glUniformMatrix4fv(glGetUniformLocation(shaderProg, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles 
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Flyer::update(glm::mat4 transform) {
	model = transform * model;
}