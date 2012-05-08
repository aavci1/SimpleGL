#version 330 core

// inputs
in vec3 POSITION;

void main() {
   gl_Position = vec4(POSITION, 1.0);
}
