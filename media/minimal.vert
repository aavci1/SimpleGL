#version 330 core

// vertex position and color
in  vec3 sglPosition;
in  vec3 sglColor;
// color to be passed to the fragment shader
out vec3 fs_in_color;
 
void main(void) {
  gl_Position = vec4(sglPosition, 1.0);
  fs_in_color = sglColor;
}