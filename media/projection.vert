#version 330

uniform mat4 sglWorldViewProjMatrix;

in vec3 sglPosition; 
in vec3 sglColor; 
out vec3 fs_in_color;

void main() { 
   gl_Position = sglWorldViewProjMatrix * vec4(sglPosition, 1.0); 
   fs_in_color = sglColor; 
}