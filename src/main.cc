

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

int main( int argc, char** argv ) {

  state world;
  world.running = true;

  int sdl_init_ = SDL_Init( SDL_INIT_VIDEO );
  if( sdl_init_ < 0 )
    std::cout << "SDL error ->" << SDL_GetError() << std::endl;

  SDL_Window* window = SDL_CreateWindow( "OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    1280, 720, SDL_WINDOW_OPENGL );

  SDL_GLContext gl_context = SDL_GL_CreateContext( window );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
  SDL_GL_SetSwapInterval( 1 );

  glewExperimental = GL_TRUE;
  GLenum glew_init_;
  if( ( glew_init_ = glewInit() ) != GLEW_OK ) {
    std::cout << glewGetErrorString( glew_init_ ) << std::endl;
    assert( false && "GLEW INIT FAILED" );
  }

  GLuint vao = 0;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  drawable teapot, plane;
  _load_obj( "teapot.obj", &teapot );
  _load_obj( "plane.obj", &plane );

  GLint program = _create_program( "vs_basic.glsl", "fs_basic.glsl" );
  if( program == -1 ) return 1;
  glUseProgram( program );

  glm::vec3 position = glm::vec3( 0.0f, 0.0f, -20.0f );
  world.view = glm::translate( glm::mat4( 1.0f ), position );
  world.projection = glm::perspective( 75.0f, 16.0f / 9.0f, 0.1f, 100.0f );

  teapot.m_location = glGetUniformLocation( program, "model" );
  if( teapot.m_location == -1 )    std::cout << "Model matrix not found in shader!" << std::endl;
  glUniformMatrix4fv( teapot.m_location, 1, false, &teapot.model[0][0] );

  plane.m_location = glGetUniformLocation( program, "model" );
  if( plane.m_location == -1 )    std::cout << "Model matrix not found in shader!" << std::endl;
  glUniformMatrix4fv( plane.m_location, 1, false, &plane.model[0][0] );

  GLint view_location = glGetUniformLocation( program, "view" );
  if( view_location == -1 )    std::cout << "View matrix not found in shader!" << std::endl;
  glUniformMatrix4fv( view_location, 1, false, &world.view[0][0] );

  GLint proj_location = glGetUniformLocation( program, "projection" );
  if( proj_location == -1 )    std::cout << "Projection matrix not found in shader!" << std::endl;
  glUniformMatrix4fv( proj_location, 1, false, &world.projection[0][0] );

  GLuint vertex_index = 0;
  glVertexAttribPointer( vertex_index, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( void * ) 0 );
  glEnableVertexAttribArray( vertex_index );
  GLuint normal_index = 1;
  glVertexAttribPointer( normal_index, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( void * ) ( sizeof( float ) * 3 ) );
  glEnableVertexAttribArray( normal_index );

  std::shared_ptr<Camera> camera = std::make_shared<Camera>();

  while( world.running ) {

    Input::get_instace()->update();
    world.running = !Input::get_instace()->quit();
    camera->update();

    world.view = glm::translate( glm::mat4( 1.0f ), position );
    teapot.model = glm::rotate( teapot.model, 0.04f, glm::vec3( 0.0f, 1.0, 0.0f ) );

    glClearColor( 0.2f, 0.5f, 0.5f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_FRONT );

    glBindVertexArray( vao );
    glUseProgram( program );
    glUniformMatrix4fv( view_location, 1, false, &camera->get_view()[0][0] );

    glUniformMatrix4fv( plane.m_location, 1, false, &plane.model[0][0] );
    glBindBuffer( GL_ARRAY_BUFFER, plane.v_buffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, plane.e_buffer );
    glDrawElements( GL_TRIANGLES, plane.e_data.size(), GL_UNSIGNED_INT, ( void* ) 0 );

    glUniformMatrix4fv( teapot.m_location, 1, false, &teapot.model[0][0] );
    glBindBuffer( GL_ARRAY_BUFFER, teapot.v_buffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, teapot.e_buffer );
    glDrawElements( GL_TRIANGLES, teapot.e_data.size(), GL_UNSIGNED_INT, ( void* ) 0 );

    SDL_GL_SwapWindow( window );
  }
  return 0;
}
