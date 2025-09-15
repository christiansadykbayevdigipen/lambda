#version 330 core

layout(location = 0) in vec3 InputPosition;

void main() {
	gl_Position = vec4(InputPosition, 1.0);
}