#version 450 core
precision mediump float;


// uniforms
uniform mat4 modelViewProjMatrix;
// inputs
in vec3 POSITION;

void main() {
  gl_Position = modelViewProjMatrix * vec4(POSITION, 1.0);
}
