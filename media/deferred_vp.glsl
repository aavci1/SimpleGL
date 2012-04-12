#version 330 core

// model view projection matrix
uniform mat4 sglModelViewProjMatrix;
// vertex position
in vec3 sglPosition; 

void main() { 
   gl_Position = sglModelViewProjMatrix * vec4(sglPosition, 1.0); 
}