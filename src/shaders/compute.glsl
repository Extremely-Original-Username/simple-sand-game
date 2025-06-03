#version 460 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
uniform sampler2D SandGridTexture;

void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    if  (pixelCoord.x != 1){
        //imageStore(SandGridTexture, pixelCoord, vec4(1.0, 0.0, 0.0, 1.0));
    }
}