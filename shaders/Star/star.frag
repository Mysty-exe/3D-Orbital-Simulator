#version 460 core

layout(location=0)out vec4 FragColor;
layout(location=1)out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D textures[16];
uniform int textureTypes[16];
uniform int textureCount;

uniform vec3 color;
uniform float intensity;

const int DIFFUSE=0;

void main()
{
    vec3 baseColor=vec3(0.);
    
    for(int i=0;i<textureCount;i++)
    {
        if(textureTypes[i]==DIFFUSE)
        baseColor+=texture(textures[i],TexCoords).rgb;
    }
    
    if(textureCount==0)
    baseColor=color;
    
    vec3 finalColor=baseColor*intensity;
    
    FragColor=vec4(finalColor,1.);
    
    float brightness=dot(finalColor,vec3(.2126,.7152,.0722));
    
    if(brightness>.2)
    BrightColor=vec4(finalColor,1.);
    else
    BrightColor=vec4(0.,0.,0.,1.);
}