#version 330 core

// model view projection matrix
uniform mat4 sglModelViewProjMatrix;
// vertex position and texture coordinate
in vec3 sglPosition; 

void main() { 
  // calculate position of the vertex in window coordinates
  gl_Position = sglModelViewProjMatrix * vec4(sglPosition, 1.0); 
}