#include "demo/gl_helper.h"
#include "GL/glew.h"
#include <iostream>

int32_t _create_program(std::string vs_shader, std::string fs_shader) {

	std::string vs_path = "../assets/shaders/" + vs_shader;
	std::string fs_path = "../assets/shaders/" + fs_shader;

	std::ifstream vs_ifstream(vs_path);
	std::string vs_str((std::istreambuf_iterator<char>(vs_ifstream)),
						std::istreambuf_iterator<char>());

	if (vs_str.size() == 0) {
		std::cout << "Vertex shader not found \n";
		return -1;
	}

	std::ifstream fs_ifstream(fs_path);
	std::string fs_str((std::istreambuf_iterator<char>(fs_ifstream)),
		std::istreambuf_iterator<char>());

	if (fs_str.size() == 0) {
		std::cout << "Fragment shader not found \n";
		return -1;
	}

	GLint vs = glCreateShader(GL_VERTEX_SHADER);
	GLint v_size = strlen(vs_str.c_str());
	const char *vs_char = vs_str.c_str();
	glShaderSource(vs, 1, &vs_char, &v_size);
	glCompileShader(vs);

	GLchar* vs_shader_log = new GLchar[1024];
	glGetShaderInfoLog(vs, 1024, nullptr, vs_shader_log);
	if (strlen(vs_shader_log) != 0) {
		std::cout << "VS shader error: \n" << vs_shader_log << std::endl;
		delete vs_shader_log;
		return -1;
	}

	GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
	GLint f_size = strlen(fs_str.c_str());
	const char *fs_char = fs_str.c_str();
	glShaderSource(fs, 1, &fs_char, &f_size);
	glCompileShader(fs);

	GLchar* fs_shader_log = new GLchar[1024];
	glGetShaderInfoLog(fs, 1024, nullptr, fs_shader_log);
	if (strlen(fs_shader_log) != 0) {
		std::cout << "FS shader error: \n" << fs_shader_log << std::endl;
		delete vs_shader_log;
		delete fs_shader_log;
		return -1;
	}

	GLint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLchar* info_log = new GLchar[1024];
	glGetProgramInfoLog(program, 1024, nullptr, info_log);
	if (strlen(info_log) != 0) {
		std::cout << "Program link error: \n" << info_log << std::endl;
		delete vs_shader_log;
		delete fs_shader_log;
		delete info_log;
		return -1;
	}

	delete vs_shader_log;
	delete fs_shader_log;
	delete info_log;

	return program;
}