#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textures[16];
uniform int textureTypes[16];
uniform int textureCount;

const int DIFFUSE=0;

void main()
{
    vec3 color=vec3(0.);
    
    for(int i=0;i<textureCount;i++)
    {
        if(textureTypes[i]==DIFFUSE)
        {
            color+=texture(textures[i],TexCoords).rgb;
        }
    }
    
    if(textureCount==0)
    {
        FragColor=vec4(1.,0.,1.,1.);
        return;
    }
    
    FragColor=vec4(color,1.);
}