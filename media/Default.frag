#version 330 core

// input
in vec4 _worldPosition;
in vec4 _normal;
// outputs
out vec4 Color;
out vec4 Normal;
out vec4 Position;

void main(void) {
  // color of the fragment
  Color = vec4(1.0, 1.0, 1.0, 1.0);
  // normal of the fragment
  Normal = vec4(_normal.xyz * 0.5 + 0.5, 0.0);
  // position of the fragment
  Position = _worldPosition;
}
