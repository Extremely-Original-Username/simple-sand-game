#version 460 core

layout (location = 0) in vec3 vectorPosition;

out vec2 TextureCoordinates;

void main()
{
   TextureCoordinates = vec2(vectorPosition.x, vectorPosition.y) * 0.5 + 0.5; // Convert from [-1, 1] to [0, 1]
   gl_Position = vec4(vectorPosition.x, vectorPosition.y, vectorPosition.z, 1.0);
}