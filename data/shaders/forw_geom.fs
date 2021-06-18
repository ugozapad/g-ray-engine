#version 330 core
out vec4 FragColor;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;

// texture samplers
uniform sampler2D u_albedoTexture;
uniform sampler2D u_normalTexture;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ObjectColor;

uniform bool u_selfillum;
uniform bool u_usingNormalMapping;

void main()
{
	if (u_selfillum)
	{
		FragColor = texture(u_albedoTexture, TexCoord);
	}
	else
	{
		// calculate normal mapping
		if (u_usingNormalMapping)
		{
			vec3 normal = texture(u_normalTexture, TexCoord).rgb;
			normal = normalize(normal * 2.0 - 1.0);
		}

		float ambientStrength = 0.1;
		vec3 ambient = ambientStrength * texture(u_albedoTexture, TexCoord).rgb;
	
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(u_LightPos - Pos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = u_LightColor * diff * texture(u_albedoTexture, TexCoord).rgb;  
	
		if (u_usingNormalMapping)
		{
			
		}

		vec3 result = ambient + diffuse;
		FragColor = vec4(result, 1.0);
	}
}