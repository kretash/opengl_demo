#pragma once

#include <stdio.h>
#include <memory>
#include <math.h>
#include <vector>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include <gl/GL.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.inl>

struct state {
	bool running;
	glm::mat4 projection;
	glm::mat4 view;
};

struct drawable {
	GLuint v_buffer;
	std::vector<glm::vec3> v_data;

	GLuint e_buffer;
	GLuint e_count;
	std::vector<uint32_t> e_data;

	GLint m_location;
	GLuint vao;
	glm::mat4 model;

	void bind_model(GLint program) {

		m_location = glGetUniformLocation(program, "model");
		if (m_location == -1)    std::cout << "Model matrix not found in shader!" << std::endl;
		glUniformMatrix4fv(m_location, 1, false, &model[0][0]);

	}
};