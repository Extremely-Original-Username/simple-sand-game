#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (r8, binding = 0) uniform image2D SandGridTexture;

void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    float currentPixelValue = imageLoad(SandGridTexture, pixelCoord).r;

    float newValue = currentPixelValue + 0.01; // Example operation, incrementing the pixel value

    if  (pixelCoord.x > 5){
        imageStore(SandGridTexture, pixelCoord, vec4(newValue, 0.0, 0.0, 1.0));
    }
}