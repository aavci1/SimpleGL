#version 330 core

// uniforms
uniform mat4 modelViewProjMatrix;
// inputs
in vec3 sglPosition;

void main() {
  gl_Position = modelViewProjMatrix * vec4(sglPosition, 1.0);
}
