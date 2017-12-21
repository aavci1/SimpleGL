#version 450 core
precision mediump float;


// uniforms
uniform sampler2D texture0;
uniform float specularIntensity;
uniform float specularPower;
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
  Color = vec4(texture2D(texture0, _texCoord).xyz, specularIntensity);
  // normal of the fragment
  Normal = vec4(_normal.xyz * 0.5 + 0.5, specularPower);
  // position of the fragment
  Position = _worldPosition;
}
