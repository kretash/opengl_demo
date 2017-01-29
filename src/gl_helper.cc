#include "demo/gl_helper.h"
#include "GL/glew.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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

void _load_obj( std::string obj, drawable* d ){

  std::string inputfile = "../assets/obj/" + obj;
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  bool ret = tinyobj::LoadObj( &attrib, &shapes, &materials, &err, inputfile.c_str() );
  if( !err.empty() ) {
    std::cout << err << std::endl;
  }

  for( size_t s = 0; s < shapes.size(); s++ ) {
    size_t index_offset = 0;
    for( size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++ ) {
      int fv = shapes[s].mesh.num_face_vertices[f];

      for( size_t v = 0; v < fv; v++ ) {
        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

        d->v_data.push_back( glm::vec3(
          attrib.vertices[3 * idx.vertex_index + 0],
          attrib.vertices[3 * idx.vertex_index + 1],
          attrib.vertices[3 * idx.vertex_index + 2]
        ) );

        d->v_data.push_back( glm::vec3(
          attrib.normals[3 * idx.normal_index + 0],
          attrib.normals[3 * idx.normal_index + 1],
          attrib.normals[3 * idx.normal_index + 2]
        ) );

        d->e_data.push_back( index_offset + v );
      }
      index_offset += fv;

    }
  }

  glGenBuffers( 1, &d->v_buffer );
  glGenBuffers( 1, &d->e_buffer );

  glBindBuffer( GL_ARRAY_BUFFER, d->v_buffer );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, d->e_buffer );

  glBufferData( GL_ARRAY_BUFFER, d->v_data.size() * sizeof( glm::vec3 ), d->v_data.data(), GL_STATIC_DRAW );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, d->e_data.size() * sizeof( uint32_t ), d->e_data.data(), GL_STATIC_DRAW );

 // glBindBuffer( GL_ARRAY_BUFFER, 0 );
  //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}