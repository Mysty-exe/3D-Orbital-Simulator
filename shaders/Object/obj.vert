#version 460 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec4 worldPos=model*vec4(aPos,1.);
    gl_Position=projection*view*worldPos;
    FragPos=worldPos.xyz;
    
    mat3 normalMatrix=mat3(transpose(inverse(model)));
    Normal=normalize(normalMatrix*aNormal);
    TexCoords=aTexCoords;
}
