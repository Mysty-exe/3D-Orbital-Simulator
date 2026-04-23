#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex1;
uniform vec4 color;
uniform bool image;
uniform float cornerRadius;
uniform vec2 rectSize;

float roundedRectSDF(vec2 p, vec2 b, float r)
{
    vec2 q = abs(p) - b + r;
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - r;
}

void main()
{
    if (image)
    {
        FragColor = texture(tex1, TexCoord);
        return;
    }

    vec2 p = (TexCoord - 0.5) * rectSize;

    float r = cornerRadius * min(rectSize.x, rectSize.y);

    vec2 halfSize = rectSize * 0.5;

    float dist = roundedRectSDF(p, halfSize, r);

    float aa = max(fwidth(dist), 1.0 / 1024.0);

    float alpha = 1.0 - smoothstep(-aa, aa, dist);

    vec3 rgb = color.rgb * alpha;

    FragColor = vec4(rgb, color.a * alpha);
}