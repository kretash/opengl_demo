

#include <Windows.h>
#include <cassert>
#include <iostream>
#include <math.h>
#include <iostream>
#include <fstream>

#include "SDL/SDL.h"
#include "demo/demo.h"
#include "demo/gl_helper.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

int main(int argc, char** argv) {

	state world;
	world.running = true;

	int sdl_init_ = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_init_ < 0)
		std::cout << "SDL error ->" << SDL_GetError() << std::endl;

	SDL_Window* window = SDL_CreateWindow("Kretash Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720, SDL_WINDOW_OPENGL);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	GLenum glew_init_;
	if ((glew_init_ = glewInit()) != GLEW_OK) {
		std::cout << glewGetErrorString(glew_init_) << std::endl;
		assert(false && "GLEW INIT FAILED");
	}

	drawable cube;

	std::string inputfile = "../assets/obj/cube.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				cube.v_data.push_back(glm::vec3(
					attrib.vertices[3 * idx.vertex_index + 0],
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2]
				));

				cube.v_data.push_back(glm::vec3(
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				));

				cube.e_data.push_back( index_offset + v);
			}
			index_offset += fv;



			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}


	glGenBuffers(1, &cube.v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube.v_buffer);
	glBufferData(GL_ARRAY_BUFFER, cube.v_data.size() * sizeof(glm::vec3), cube.v_data.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &cube.e_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.e_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.e_data.size() * sizeof(uint32_t), cube.e_data.data(), GL_STATIC_DRAW);

	GLint program = _create_program("vs_basic.glsl", "fs_basic.glsl");
	if (program == -1) return 1;
	glUseProgram(program);

	glm::vec3 position = glm::vec3(0.0f, 0.0f, -20.0f); 
	world.view = glm::translate(glm::mat4(1.0f), position);
	world.projection = glm::perspective(75.0f, 16.0f / 9.0f, 0.1f, 100.0f);

	cube.m_location = glGetUniformLocation(program, "model");
	if (cube.m_location == -1)    std::cout << "Model matrix not found in shader!" << std::endl;
	glUniformMatrix4fv(cube.m_location, 1, false, &cube.model[0][0]);

	GLint view_location = glGetUniformLocation(program, "view");
	if (view_location == -1)    std::cout << "View matrix not found in shader!" << std::endl;
	glUniformMatrix4fv(view_location, 1, false, &world.view[0][0]);

	GLint proj_location = glGetUniformLocation(program, "projection");
	if (proj_location == -1)    std::cout << "Projection matrix not found in shader!" << std::endl;
	glUniformMatrix4fv(proj_location, 1, false, &world.projection[0][0]);

	GLuint vertex_index = 0;
	glVertexAttribPointer(vertex_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
	glEnableVertexAttribArray(vertex_index);
	GLuint normal_index = 1;
	glVertexAttribPointer(normal_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
	glEnableVertexAttribArray(normal_index);

	while (world.running) {
		SDL_Event event_;
		while (SDL_PollEvent(&event_)) {
			switch (event_.type) {
			case SDL_QUIT:
				world.running = false;
				break;
			case SDL_KEYDOWN:
				switch (event_.key.keysym.sym) {
				case SDLK_w:
					position.z += 0.1f;
					break;
				case SDLK_a:
					position.x -= 0.1f;
					break;
				case SDLK_s:
					position.z -= 0.1f;
					break;
				case SDLK_d:
					position.x += 0.1f;
					break;
				default:
					break;
				}
			default:
				break;
			}
		}


		world.view = glm::translate(glm::mat4(1.0f), position);
		glUniformMatrix4fv(view_location, 1, false, &world.view[0][0]);

		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glBindBuffer(GL_ARRAY_BUFFER, cube.v_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.e_buffer);

		cube.model = glm::rotate(cube.model, 0.04f, glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(cube.m_location, 1, false, &cube.model[0][0]);

		glDrawElements(GL_TRIANGLES, cube.e_data.size(), GL_UNSIGNED_INT, (void*)0);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}
