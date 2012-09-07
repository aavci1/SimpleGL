#version 330 core

// uniforms
uniform mat4 modelViewProjMatrix;
uniform mat4 modelMatrix;
// inputs
in vec3 POSITION;
in vec3 NORMAL;
in vec2 TEXCOORD0;

// outputs
out vec4 _worldPosition;
out vec4 _normal;
out vec2 _texCoord;

void main() {
   gl_Position = modelViewProjMatrix * vec4(POSITION, 1.0);
   // pass output
   _worldPosition = modelMatrix * vec4(POSITION, 1.0);
   _normal = normalize(modelMatrix * vec4(NORMAL, 0.0));
   _texCoord = TEXCOORD0;
}
