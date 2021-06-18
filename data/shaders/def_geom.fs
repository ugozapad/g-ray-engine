#version 330 core
layout (location = 0) out vec3 GPosition;
layout (location = 1) out vec3 GNormal;
layout (location = 2) out vec4 GAlbedoSpec;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D u_albedoTexture;

void main()
{
    GPosition = Pos;
    GNormal = normalize(Normal);
    GAlbedoSpec.rgb = texture(u_albedoTexture, TexCoord).rgb;
    //AlbedoSpec.a = texture(texture_specular1, TexCoord).r;
}