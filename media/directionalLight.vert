#version 330 core

// inputs
in vec3 sglPosition; 

void main() { 
   gl_Position = vec4(sglPosition, 1.0); 
}
