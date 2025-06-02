#version 460 core

layout (location = 0) in vec3 vectorPosition;

void main()
{
   gl_Position = vec4(vectorPosition.x, vectorPosition.y, vectorPosition.z, 1.0);
}