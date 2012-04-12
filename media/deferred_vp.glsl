#version 330 core

// model view projection matrix
uniform mat4 sglModelViewProjMatrix;
// inputs
in vec3 sglPosition; 
in vec3 sglNormal;
in vec2 sglTexCoord0;
// outputs
out vec4 _worldPosition;
out vec4 _normal;
out vec2 _texCoord;

void main() { 
   gl_Position = sglModelViewProjMatrix * vec4(sglPosition, 1.0); 
   // pass output
   _worldPosition = sglModelViewProjMatrix * vec4(sglPosition, 1.0);
   _normal = normalize(sglModelViewProjMatrix * vec4(sglNormal, 0.0));
   _texCoord = sglTexCoord0;
}
