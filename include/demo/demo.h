#pragma once

#include <stdio.h>
#include <memory>
#include <math.h>
#include <vector>

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
	std::vector<uint32_t> e_data;

	GLint m_location;
	glm::mat4 model;
};