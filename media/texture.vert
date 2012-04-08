#version 330 core

// model view projection matrix
uniform mat4 sglModelViewProjMatrix;
// vertex position and texture coordinate
in vec3 sglPosition; 
in vec2 sglTexCoord0;
// texture coordinate passed to the fragment shader
out vec2 fs_in_texCoord0;

void main() { 
  // calculate position of the vertex in window coordinates
  gl_Position = sglModelViewProjMatrix * vec4(sglPosition, 1.0); 
  // pass the texture coordinate to the fragment shader
  fs_in_texCoord0 = sglTexCoord0;
}