#version 330 core

// model view projection matrix
uniform mat4 sglModelViewProjMatrix;
// vertex position and color
in vec3 sglPosition; 
in vec3 sglColor;
// color passed to the fragment shader
out vec3 fs_in_color;

void main() { 
  // calculate position of the vertex in window coordinates
  gl_Position = sglModelViewProjMatrix * vec4(sglPosition, 1.0); 
  // pass the color to the fragment shader
  fs_in_color = sglColor;
}
