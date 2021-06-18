#version 330 core

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D u_texture;

void main()
{
    fragColor = vec4(texture(u_texture, texCoord).rgb, 1.0);
    //fragColor = vec4(1.0, 0.0, 0.0, 0.0);
}