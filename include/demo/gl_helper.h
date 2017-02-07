#pragma once
#include <stdint.h>
#include <string>
#include <fstream>
#include <streambuf>
#include "demo/demo.h"

int32_t _create_program(std::string vs_shader, std::string fs_shader);
void _load_obj( std::string obj, drawable* d );
void _load_obj_chunk(std::string obj, std::vector<float>* v_buffer, glm::vec3 pos);
void _upload_vertices(std::vector<float>* v_data, GLint* v_buffer, GLuint* vao);