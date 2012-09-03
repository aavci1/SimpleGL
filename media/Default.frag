#version 330 core

// uniforms
uniform float specularIntensity;
uniform float specularPower;
// input
in vec4 _worldPosition;
in vec4 _normal;
// outputs
out vec4 Color;
out vec4 Normal;
out vec4 Position;

void main(void) {
  // color of the fragment
  Color = vec4(1.0, 1.0, 1.0, specularIntensity);
  // normal of the fragment
  Normal = vec4(_normal.xyz * 0.5 + 0.5, specularPower);
  // position of the fragment
  Position = _worldPosition;
}
