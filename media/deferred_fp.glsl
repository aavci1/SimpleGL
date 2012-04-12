#version 330 core

// outputs
out vec4 Color;
out vec4 Normal;
out vec4 Position;

void main(void) {
  // color of the fragment
  Color = vec4(1.0, 0.0, 0.0, 1.0);
  // normal of the fragment
  Normal = vec4(0.0, 1.0, 0.0, 1.0);
  // position of the fragment
  Position = vec4(0.0, 0.0, 1.0, 1.0);
}
