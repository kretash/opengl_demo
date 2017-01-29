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

#if 0
struct vec3 {
	float x, y, z;
};

struct _vec3 {
	static vec3 zero() {
		vec3 v;
		v.x = 0.0f; v.y = 0.0f; v.z = 0.0f;
		return v;
	}
	static vec3 set(float x, float y, float z) {
		vec3 v;
		v.x = x; v.y = y; v.z = z;
		return v;
	}
};

struct ivec3 {
	int x, y, z;
};
struct _ivec3 {
	static ivec3 zero() {
		ivec3 v;
		v.x = 0; v.y = 0; v.z = 0;
		return v;
	}
	static ivec3 set(int x, int y, int z) {
		ivec3 v;
		v.x = x; v.y = y; v.z = z;
		return v;
	}
};

struct mat4 {
	float data[16];

	mat4() {
		float tmp[16]{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		memcpy(data, tmp, sizeof(float) * 16);
	}

	void identity() {
		mat4();
	}

	void translate(float x, float y, float z) {
		data[12] = x;
		data[13] = y;
		data[14] = z;
	}

	void rotate_y(float a) {
		data[0] = cos(a);
		data[2] = sin(a);
		data[8] = -sin(a);
		data[10] = cos(a);
	}

	void prespective(float fov, float aspect_ratio, float n, float f) {
		float tan_fov = tan(fov / 2.0f);
		mat4();
		data[0] = 1.0f / (aspect_ratio*tan_fov);
		data[5] = 1.0f / tan_fov;
		data[10] = -(f + n) / (f - n);
		data[11] = -1.0f;
		data[14] = -(2.0f * f * n) / (f - n);
	}

	float* get() {
		return data;
	}
};
#endif // 0


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