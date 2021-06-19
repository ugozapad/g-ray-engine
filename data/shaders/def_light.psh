#version 330 core
#extension GL_ARB_texture_rectangle: enable

out vec4 fragColor;
in vec2 texCoord;

uniform sampler2D u_positionTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_colorTexture;

// light stuff
uniform vec3 u_lightPos;

uniform vec3 u_viewPos;

void main(void)
{
	vec3 pos = texture(u_positionTexture, texCoord).rgb;
	vec3 normal = texture(u_normalTexture, texCoord).rgb;
	vec3 color = texture(u_colorTexture, texCoord).rgb;
	
	vec3 resultOfLighting = color * 0.1;
	vec3 viewDir = normalize(u_viewPos - pos); 
	
	vec3 lightDir = normalize(u_lightPos - pos);
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * color * vec3(1.0, 1.0, 1.0); 
	resultOfLighting += diffuse;
	
	fragColor = vec4(resultOfLighting, 1.0);
}