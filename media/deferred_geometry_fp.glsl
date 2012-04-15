#version 330 core

// uniforms
uniform sampler2D sglSampler;
uniform float sglSpecularIntensity;
uniform float sglSpecularPower;
// input
in vec4 _worldPosition;
in vec4 _normal;
in vec2 _texCoord;
// outputs
out vec4 Color;
out vec4 Normal;
out vec4 Position;

void main(void) {
  // color of the fragment
  Color = texture2D(sglSampler, _texCoord);
  // normal of the fragment
  Normal = vec4(_normal.xyz, sglSpecularIntensity);
  // position of the fragment
  Position = vec4(_worldPosition.xyz, sglSpecularPower);
}
