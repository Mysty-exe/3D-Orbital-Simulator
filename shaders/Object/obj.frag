#version 460 core

layout(location=0)out vec4 FragColor;
layout(location=1)out vec4 BrightColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

#define MAX_LIGHTS 16

uniform sampler2D textures[16];
uniform int textureTypes[16];
uniform int textureCount;

uniform int lightCount;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];

uniform vec3 viewPos;

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
    baseColor=vec3(.7);
    
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);
    
    vec3 result=vec3(0.);
    
    result+=.02*baseColor;
    
    for(int i=0;i<lightCount;i++)
    {
        vec3 lightDir=normalize(lightPositions[i]-FragPos);
        
        float distance=length(lightPositions[i]-FragPos);
        
        float attenuation=1./(distance*distance*.0000001);
        attenuation=max(attenuation,.001);
        
        float diff=max(dot(norm,lightDir),0.);
        vec3 diffuse=diff*baseColor*lightColors[i];
        
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec=pow(max(dot(norm,halfwayDir),0.),32.);
        vec3 specular=spec*lightColors[i]*.3;
        
        result+=(diffuse+specular)*attenuation;
    }
    
    FragColor=vec4(result,1.);
    
    float brightness=dot(result,vec3(.2126,.7152,.0722));
    
    if(brightness>.2)
    BrightColor=vec4(result,1.);
    else
    BrightColor=vec4(0.,0.,0.,1.);
}