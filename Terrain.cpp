#include "Terrain.h"

#include <iostream>

Terrain::Terrain(int size, GLuint shaderProgram) : size(size), shaderProg(shaderProgram) {
	std::vector<float> v(size, 0.0f);
	heights = std::vector<std::vector<float>>(size, v);
}

void Terrain::generateHeights() {
	int scale = 50;
	int scale2 = 2 * scale;
	int scale3 = 4 * scale;
	srand(time(NULL));
	PerlinNoise noiseGen(rand() % 1000);
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			double nx = (double)x / size - 0.5;
			double ny = (double)y / size - 0.5;
			
			heights[x][y] = pow(noiseGen.noise(scale * nx, scale * ny, 1) +
				0.5 * noiseGen.noise(scale2 * nx, scale2 * ny, 1) +
				0.25 * noiseGen.noise(scale3 * nx, scale2 * ny, 1), 2.2);
			
		}
	}
}

void Terrain::genTriangles() {
	// generate vertices
	std::vector<std::vector<glm::vec3>> vertices2D = std::vector<std::vector<glm::vec3>>(size, std::vector<glm::vec3>(size));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			float factorRow = 100*(float(i) / float(size - 1));
			float factorCol = 100*(float(j) / float(size - 1));
			vertices2D[i][j] = glm::vec3(-0.5f + factorCol, heights[i][j], -0.5f + factorRow);
		}
		vertices.insert(vertices.end(), vertices2D[i].begin(), vertices2D[i].end());
	}

	// generate normals
	std::vector<std::vector<glm::vec3>> normals2D = std::vector<std::vector<glm::vec3>>(size, std::vector<glm::vec3>(size));
	std::vector<std::vector<glm::vec3>> tempNormals[2];
	for (auto i = 0; i < 2; i++) {
		tempNormals[i] = std::vector<std::vector<glm::vec3>>(size - 1, std::vector<glm::vec3>(size - 1));
	}

	for (auto i = 0; i < size - 1; i++)
	{
		for (auto j = 0; j < size - 1; j++)
		{
			const auto& vertexA = vertices2D[i][j];
			const auto& vertexB = vertices2D[i][j + 1];
			const auto& vertexC = vertices2D[i + 1][j + 1];
			const auto& vertexD = vertices2D[i + 1][j];

			const auto triangleNormalA = glm::cross(vertexB - vertexA, vertexA - vertexD);
			const auto triangleNormalB = glm::cross(vertexD - vertexC, vertexC - vertexB);

			tempNormals[0][i][j] = glm::normalize(triangleNormalA);
			tempNormals[1][i][j] = glm::normalize(triangleNormalB);
		}
	}

	for (auto i = 0; i < size; i++)
	{
		for (auto j = 0; j < size; j++)
		{
			const auto isFirstRow = i == 0;
			const auto isFirstColumn = j == 0;
			const auto isLastRow = i == size - 1;
			const auto isLastColumn = j == size - 1;

			auto finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for triangle to the upper-lef
			if (!isFirstRow && !isFirstColumn) {
				finalVertexNormal += tempNormals[0][i - 1][j - 1];
			}

			// Look for triangles to the upper-righ
			if (!isFirstRow && !isLastColumn) {
				for (auto k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][i - 1][j];
				}
			}

			// Look for triangle to the bottom-righ
			if (!isLastRow && !isLastColumn) {
				finalVertexNormal += tempNormals[0][i][j];
			}

			// Look for triangles to the bottom-righ
			if (!isLastRow && !isFirstColumn) {
				for (auto k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][i][j - 1];
				}
			}

			// Store final normal of j-th vertex in i-th ro
			normals2D[i][j] = glm::normalize(finalVertexNormal);
		}
		normals.insert(normals.end(), normals2D[i].begin(), normals2D[i].end());
	}
	
	// generate indices
	GLuint restartIndex = size * size;

	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < 2; k++) {
				indices.push_back((i + k) * size + j);
			}
		}
		indices.push_back(restartIndex);
	}

	std::vector<std::vector<glm::vec2>> textures2D = std::vector<std::vector<glm::vec2>>(size, std::vector<glm::vec2>(size));

	const auto textureStepU = 0.1f;
	const auto textureStepV = 0.1f;

	for (auto i = 0; i < size; i++)
	{
		for (auto j = 0; j < size; j++) {
			textures2D[i][j] = glm::vec2(textureStepU * j, textureStepV * i);
		}
		textureCord.insert(textureCord.end(), textures2D[i].begin(), textures2D[i].end());
	}
}

void Terrain::init() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
		normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textureCord.size(),
		textureCord.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(shaderProg);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	std::string filename = "textures/grass.jpg";
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Terrain::draw() {
	glUseProgram(shaderProg);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProg, "texture0"), 10);

	glBindVertexArray(vao);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(size * size);
	glDrawElements(GL_TRIANGLE_STRIP, (size - 1) * size * 2 + size - 1, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);
}