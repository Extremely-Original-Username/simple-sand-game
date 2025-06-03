#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (r8, binding = 0) uniform image2D SandGridTexture;
uniform ivec2 CreateSandPosition;

void main()
{
    ivec2 currentPixelCoord = ivec2(gl_GlobalInvocationID.xy);
    float currentPixelValue = imageLoad(SandGridTexture, currentPixelCoord).r;

    //Awful implementation, put this somewhere else and dont check EVERY pixel
    if(abs(currentPixelCoord.x - CreateSandPosition.x) <= 1  && abs(currentPixelCoord.y - CreateSandPosition.y) <= 1){
        imageStore(SandGridTexture, currentPixelCoord, vec4(1.0, 0.0, 0.0, 1.0));
    }

    /*Physics logic*/
    ivec2 belowPixelCoord = currentPixelCoord + ivec2(0, -1);
    float belowPixelValue = imageLoad(SandGridTexture, belowPixelCoord).r;

    if (currentPixelValue == 1 && belowPixelCoord.y < imageSize(SandGridTexture).y && belowPixelCoord.y >= 0){
        if (belowPixelValue == 0) {
            imageStore(SandGridTexture, currentPixelCoord, vec4(0.0, 0.0, 0.0, 1.0));
            imageStore(SandGridTexture, belowPixelCoord, vec4(1.0, 0.0, 0.0, 1.0));
            return;
        }
        ivec2 leftBelowPixelCoord = currentPixelCoord + ivec2(-1, -1);
        float leftBelowPixelValue = imageLoad(SandGridTexture, leftBelowPixelCoord).r;
        ivec2 rightBelowPixelCoord = currentPixelCoord + ivec2(1, -1);
        float rightBelowPixelValue = imageLoad(SandGridTexture, rightBelowPixelCoord).r;

        //TODO - Randomise this
        if (leftBelowPixelCoord.x > 0 && leftBelowPixelValue == 0){
            imageStore(SandGridTexture, currentPixelCoord, vec4(0.0, 0.0, 0.0, 1.0));
            imageStore(SandGridTexture, leftBelowPixelCoord, vec4(1.0, 0.0, 0.0, 1.0));
            return;
        }
        if (rightBelowPixelCoord.x < imageSize(SandGridTexture).x && rightBelowPixelValue == 0){
            imageStore(SandGridTexture, currentPixelCoord, vec4(0.0, 0.0, 0.0, 1.0));
            imageStore(SandGridTexture, rightBelowPixelCoord, vec4(1.0, 0.0, 0.0, 1.0));
            return;
        }
    }
}