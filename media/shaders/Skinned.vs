#version 330 core

// uniforms
uniform mat4 modelViewProjMatrix;
uniform mat4 modelMatrix;
uniform mat4 boneTransforms[100];
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
   mat4 boneTransform = boneTransforms[BONEIDS[0]] * BONEWEIGHTS[0] + boneTransforms[BONEIDS[1]] * BONEWEIGHTS[1] + boneTransforms[BONEIDS[2]] * BONEWEIGHTS[2] + boneTransforms[BONEIDS[3]] * BONEWEIGHTS[3];
   gl_Position = modelViewProjMatrix * boneTransform * vec4(POSITION, 1.0);
   // pass output
   _worldPosition = modelMatrix * boneTransform * vec4(POSITION, 1.0);
   _normal = normalize(modelMatrix * boneTransform * vec4(NORMAL, 0.0));
   _texCoord = TEXCOORD0;
}
