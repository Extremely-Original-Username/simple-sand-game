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
   int screenPercentX = int(screenPos.x * 100.0f);
   int screenPercentY = int(screenPos.y * 100.0f);

   /*FragColor = vec4(float(screenPercentY * 5 % 100) / 100, float(screenPercentX * 5 % 100) / 100, 0.5f, 1.0f);*/
   FragColor = texture(SandGridTexture, TextureCoordinates);
}