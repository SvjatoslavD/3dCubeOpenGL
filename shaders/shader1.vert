#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;

out vec4 ourColor;

uniform float aColor;

void main() {
   gl_Position = vec4(aPos, 1.0);
   ourColor = vec4(0.0, aColor, 0.0, 1.0);
}