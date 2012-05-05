#version 330 core

// model view projection matrix
uniform mat4 sglModelViewProjMatrix;
uniform mat4 sglModelMatrix;
// inputs
in vec3 POSITION;
in vec3 NORMAL;
in vec2 TEXCOORD0;
// outputs
out vec4 _worldPosition;
out vec4 _normal;
out vec2 _texCoord;

void main() {
   gl_Position = sglModelViewProjMatrix * vec4(POSITION, 1.0);
   // pass output
   _worldPosition = sglModelMatrix * vec4(POSITION, 1.0);
   _normal = normalize(sglModelMatrix * vec4(NORMAL, 0.0));
   _texCoord = TEXCOORD0;
}
