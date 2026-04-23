#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 FragPos[];
in vec3 Normal[];
in vec2 TexCoords[];

out vec3 gsFragPos;
out vec3 gsNormal;
out vec2 gsTexCoords;

void main()
{
    for (int i = 0; i < 3; i++)
    {
        gsFragPos = FragPos[i];
        gsNormal = Normal[i];
        gsTexCoords = TexCoords[i];

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}