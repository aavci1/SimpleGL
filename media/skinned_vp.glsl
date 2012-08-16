#version 330 core

// uniforms
uniform mat4 ModelViewProjMatrix;
uniform mat4 ModelMatrix;
uniform mat4 Bones[100];
// inputs
in vec3 POSITION;
in vec3 NORMAL;
in vec2 TEXCOORD0;
in ivec4 BONEIDS;
in vec4 BONEWEIGHTS;

// outputs
out vec4 _worldPosition;
out vec4 _normal;
out vec2 _texCoord;

void main() {
   mat4 boneTransform = Bones[BONEIDS[0]] * BONEWEIGHTS[0] + Bones[BONEIDS[1]] * BONEWEIGHTS[1] + Bones[BONEIDS[2]] * BONEWEIGHTS[2] + Bones[BONEIDS[3]] * BONEWEIGHTS[3];
   gl_Position = ModelViewProjMatrix * boneTransform * vec4(POSITION, 1.0);
   // pass output
   _worldPosition = ModelMatrix * vec4(POSITION, 1.0);
   _normal = normalize(ModelMatrix * boneTransform * vec4(NORMAL, 0.0));
   _texCoord = TEXCOORD0;
}
