#version 420
 
in  vec3 fs_in_color;
out vec4 fs_out_color;
 
void main(void) {
  fs_out_color = vec4(fs_in_color, 0.0);
}