#pragma once

#include <stdint.h>
#include <memory>

#include "demo.h"
#include "gl_helper.h"

class Chunk : public drawable{
public:
	Chunk(size_t size) : m_size(size)  {}
	~Chunk() {}

	void generate() {
		if (m_buffer != nullptr) return;

		m_buffer = (uint8_t***)malloc(m_size*m_size*m_size);

		for (size_t i = 0; i < m_size; ++i) {
			for (size_t e = 0; e < m_size; ++e) {
				for (size_t o = 0; o < m_size; ++o) {

					m_buffer[i][e][o] = (e % i == o || o % e == i || i % o == e) ? 1 : 0;

				}
			}
		}


		std::vector<float> v_data;

		for (size_t i = 0; i < m_size; ++i) {
			for (size_t e = 0; e < m_size; ++e) {
				for (size_t o = 0; o < m_size; ++o) {

					_load_obj_chunk("cube.obj", &v_data, glm::vec3(i, e, o));

				}
			}
		}

		_upload_vertices(&v_data, &this->v_buffer, &this->vao);



	}

private:
	size_t m_size;
	uint8_t*** m_buffer = nullptr;
};