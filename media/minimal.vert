#version 420
 
in  vec3 sglPosition;
in  vec3 sglColor;
out vec3 fs_in_color;
 
void main(void) {
  gl_Position = vec4(sglPosition, 1.0);
  fs_in_color = sglColor;
}