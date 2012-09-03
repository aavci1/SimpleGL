#version 330 core

// uniforms
uniform mat4 ModelViewProjMatrix;
uniform mat4 ModelMatrix;
// inputs
in vec3 POSITION;
in vec3 NORMAL;
// outputs
out vec4 _worldPosition;
out vec4 _normal;

void main() {
   gl_Position = ModelViewProjMatrix * vec4(POSITION, 1.0);
   // pass output
   _worldPosition = ModelMatrix * vec4(POSITION, 1.0);
   _normal = normalize(ModelMatrix * vec4(NORMAL, 0.0));
}
