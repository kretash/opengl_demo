#version 330

in float diffuse;
in vec3 frag_normal; 
out vec4 gl_FragColor; 

void main(){ 
    vec4 color = vec4( 0.3f, 0.3f, 0.8f, 1.0f); 
    gl_FragColor = color * diffuse;
} 