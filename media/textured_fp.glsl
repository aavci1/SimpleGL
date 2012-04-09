#version 330 core

// sampler to use for texturing
uniform sampler2D sampler;
// texture coordinate passed from the vertex shader
in  vec2 fs_in_texCoord0;
// final color of the fragment
out vec4 fs_out_color;

void main(void) {
  // calculate fragment color using sampler
  fs_out_color = texture2D(sampler, fs_in_texCoord0);
}