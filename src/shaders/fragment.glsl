#version 460 core

in vec2 TextureCoordinates;
out vec4 FragColor;

uniform vec2 MainWindowResolution;
uniform vec2 SandGridResolution;

uniform sampler2D SandGridTexture;

void main()
{
   /*Screen position between 0 and 1*/
   vec2 screenPos = gl_FragCoord.xy / MainWindowResolution;
   
   int screenPixelX = int(screenPos.x * MainWindowResolution.x);
   int screenPixelY = int(screenPos.y * MainWindowResolution.y);

   vec2 scale = vec2(MainWindowResolution.x / SandGridResolution.x, MainWindowResolution.y / SandGridResolution.y);

   float pixelValue = texelFetch(SandGridTexture, ivec2(screenPixelX / scale.x, screenPixelY / scale.y), 0).r;

   FragColor = vec4(pixelValue, pixelValue, pixelValue, 1.0);
}