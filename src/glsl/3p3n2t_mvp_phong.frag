#version 460 core

in vec3 vFragPos;
in vec3 vNorm;
in vec2 vTexCoords;

uniform vec3  uLightPos;
uniform vec4  uLightColor;
uniform float uLightAmbientIntensity;
uniform vec3  uCameraPos;

uniform sampler2D albedoTex;
uniform sampler2D specularTex;

out vec4 FragColor;

void main()
{
vec4 ambientLight;
vec3 lightDir;
float diff;
vec3 norm;
vec4 diffuse;
vec3 viewDir;
vec3 reflectDir;
float spec;
vec4 specular;

lightDir = normalize(uLightPos - vFragPos);  
norm = normalize(vNorm);

/* Calculate ambient light component */
ambientLight = uLightColor * texture(albedoTex, vTexCoords) * uLightAmbientIntensity;

/* Calculate diffuse light component */
diff = max(dot(norm, lightDir), 0.0);
diffuse = diff * texture(albedoTex, vTexCoords);

/* Calculate specular highlights */
viewDir = normalize(uCameraPos - vFragPos);
reflectDir = reflect(-lightDir, norm);
spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
specular = texture(specularTex, vTexCoords) * spec * uLightColor;

FragColor = (diffuse + ambientLight + specular);

} 
