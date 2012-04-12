#version 330 core

// uniforms
uniform sampler2D sglSampler;
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
  Normal = _normal;
  // position of the fragment
  Position = _worldPosition;
}
