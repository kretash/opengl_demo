#version 330

in vec3 light_pos;
in vec3 frag_normal; 
out vec4 gl_FragColor; 

void main(){ 
    vec4 color = vec4( 0.3f, 0.3f, 0.8f, 1.0f); 
    float diffuse = max( 0.4f, dot( light_pos, frag_normal )); 
    gl_FragColor = color * diffuse;
} 