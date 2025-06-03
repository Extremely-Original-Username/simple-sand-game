#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (r8, binding = 0) uniform image2D SandGridTexture;

void main()
{
    ivec2 currentPixelCoord = ivec2(gl_GlobalInvocationID.xy);
    float currentPixelValue = imageLoad(SandGridTexture, currentPixelCoord).r;

    /*Physics logic*/
    ivec2 belowPixelCoord = currentPixelCoord + ivec2(0, -1);
    float belowPixelValue = imageLoad(SandGridTexture, belowPixelCoord).r;

    if (currentPixelValue == 1 && belowPixelCoord.y < imageSize(SandGridTexture).y && belowPixelCoord.y >= 0 && belowPixelValue == 0) {
        imageStore(SandGridTexture, currentPixelCoord, vec4(0.0, 0.0, 0.0, 1.0));
        imageStore(SandGridTexture, belowPixelCoord, vec4(1.0, 0.0, 0.0, 1.0));
    }

    /*imageStore(SandGridTexture, currentPixelCoord, vec4(newValue, 0.0, 0.0, 1.0));*/
}