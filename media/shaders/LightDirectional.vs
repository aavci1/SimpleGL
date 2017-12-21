#version 450 core
precision mediump float;


// inputs
in vec3 POSITION;

void main() {
   gl_Position = vec4(POSITION, 1.0);
}
