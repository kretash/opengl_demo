

#include <Windows.h>
#include <cassert>
#include <iostream>
#include <math.h>
#include <iostream>
#include <fstream>

#include "SDL/SDL.h"
#include "demo/demo.h"
#include "demo/gl_helper.h"
#include "demo/input.h"
#include "demo/camera.h"

int main(int argc, char** argv) {

	state world;
	world.running = true;

	int sdl_init_ = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_init_ < 0)
		std::cout << "SDL error ->" << SDL_GetError() << std::endl;

	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

	std::vector<drawable> render_bin;

	const int cubes = 32;

	for (int i = 0; i < cubes; ++i) {
		for (int e = 0; e < cubes; ++e) {

			drawable cube;

			//if ((e + i) % 2 == 0)
			_load_obj("cube.obj", &cube);
			//else
			//	_load_obj("sphere.obj", &cube);

			cube.model = glm::translate(cube.model, glm::vec3(1.15*i, 0.0f, 1.5f*e));
			render_bin.push_back(cube);
		}
	}

	drawable plane;
	_load_obj("plane.obj", &plane);
	render_bin.push_back(plane);

	GLint program = _create_program("vs_basic.glsl", "fs_basic.glsl");
	if (program == -1) return 1;
	glUseProgram(program);

	glm::vec3 position = glm::vec3(0.0f, 0.0f, -20.0f);
	world.view = glm::translate(glm::mat4(1.0f), position);
	world.projection = glm::perspective(75.0f, 16.0f / 9.0f, 0.1f, 100.0f);

	for (int i = 0; i < render_bin.size(); ++i) {
		render_bin[i].bind_model(program);
	}

	GLint view_location = glGetUniformLocation(program, "view");
	if (view_location == -1)    std::cout << "View matrix not found in shader!" << std::endl;
	glUniformMatrix4fv(view_location, 1, false, &world.view[0][0]);

	GLint proj_location = glGetUniformLocation(program, "projection");
	if (proj_location == -1)    std::cout << "Projection matrix not found in shader!" << std::endl;
	glUniformMatrix4fv(proj_location, 1, false, &world.projection[0][0]);


	for (int i = 0; i < render_bin.size(); ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, render_bin[i].v_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_bin[i].e_buffer);
	}

	/*
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	*/

	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	float time = 0.0f;

	while (world.running) {

		Input::get_instace()->update();
		world.running = !Input::get_instace()->quit();
		camera->update();

		world.view = glm::translate(glm::mat4(1.0f), position);

		for (int i = 0; i < cubes; ++i) {
			for (int e = 0; e < cubes; ++e) {
				render_bin[i * 10 + e].model = glm::translate(glm::mat4(1.0f),
					glm::vec3(1.15f*i, 0.3f*sinf(time*3.0f + (float)i / 10.0f + (float)e / 10.0f), 1.15f*e));
			}
		}

		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glUseProgram(program);
		glUniformMatrix4fv(view_location, 1, false, &camera->get_view()[0][0]);

		for (int i = 0; i < render_bin.size(); ++i) {
			if (i == 0) {
				glBindVertexArray(render_bin[i].vao);
				glBindBuffer(GL_ARRAY_BUFFER, render_bin[i].v_buffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_bin[i].e_buffer);
			}
				render_bin[i].bind_model(program);
			glDrawElements(GL_TRIANGLES, render_bin[i].e_count, GL_UNSIGNED_INT, (void*)0);

		}

		SDL_GL_SwapWindow(window);
		time += 0.016f;
	}
	return 0;
}
