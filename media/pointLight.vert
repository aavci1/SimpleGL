#version 330 core

// uniforms
uniform mat4 viewProjMatrix;
uniform mat4 lightMatrix;
// inputs
in vec3 sglPosition;

void main() {
  gl_Position = viewProjMatrix * lightMatrix * vec4(sglPosition, 1.0);
}
