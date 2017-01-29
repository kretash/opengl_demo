#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out float diffuse;
out vec3 frag_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec3 light_pos = vec3( -19.0f, -31.0f, 5.3f);
    light_pos = normalize(light_pos); 
    diffuse = max( 0.4f, dot( light_pos, normal )); 
    frag_normal = normal; 

    vec4 view_coords = view * model * vec4( position, 1.0f ); 

    gl_Position = projection * view_coords;
}