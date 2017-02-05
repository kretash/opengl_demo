#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 light_pos;
out vec3 frag_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    light_pos = vec3( -19.0f, -31.0f, 5.3f);
    light_pos = normalize(light_pos); 
    frag_normal = (model  * vec4( normal, 0.0f )).xyz;  

    vec4 view_coords = view * model * vec4( position, 1.0f );
    gl_Position = projection * view_coords;
}