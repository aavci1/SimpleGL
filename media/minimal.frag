#version 330 core

// color passed from the vertex shader
in  vec3 fs_in_color;
// final color of the fragment
out vec4 fs_out_color;
 
void main(void) {
  fs_out_color = vec4(fs_in_color, 0.0);
}