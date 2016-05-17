#version 330 core

in vec3 position;

uniform mat3 trans; // the transformation matrix

void main() {
  gl_Position = vec4(trans * position, 1.0f);
}
