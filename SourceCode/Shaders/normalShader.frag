#version 330 core
out vec4 FragColor;

in vec4 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D text;

uniform vec3 camPos;
uniform int nbDirLights;
uniform int nbPointsLights;
uniform int nbSpotsLights;


// Directionnal Lights
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_DIR_LIGHTS 2
uniform DirLight dirLights[NR_DIR_LIGHTS];
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 

//Point Lights
struct PointLight {    
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;  
};  
#define NR_POINTS_LIGHTS 4
uniform PointLight pointLights[NR_POINTS_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//Spot Lights
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cosInerAngle;
    float cosOuterAngle;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINTS_LIGHTS 4
uniform SpotLight spotLights[NR_POINTS_LIGHTS];
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 colorLight = vec3(0,0,0);
    vec3 norm = normalize(Normal);
    vec3 fragPos = vec3(FragPos);
    vec3 view = normalize(camPos - fragPos);

    for(int i = 0; i < nbDirLights; i++)
        colorLight += CalcDirLight(dirLights[i],norm,view);

    for(int i = 0; i < nbPointsLights; i++)
        colorLight += CalcPointLight(pointLights[i],norm,fragPos,view);

    for(int i = 0; i < nbSpotsLights; i++)
        colorLight += CalcSpotLight(spotLights[i],norm,fragPos,view);

    FragColor = texture(text, TexCoord);
	FragColor = FragColor * vec4(colorLight,1);
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    //ambient 
    vec3 ambient = light.ambient;

    // diffuse shading
    float Id = max(dot(lightDir,normal), 0.0);
    vec3 diffuse  = light.diffuse  * Id;

    // specular shading
    float Is = pow(max(dot(halfwayDir, normal), 0.0), 256);
    vec3 specular = light.specular * Is;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  

    //ambient 
    vec3 ambient = light.ambient;

    // diffuse
    float Id = max(dot(lightDir,normal), 0.0);
    vec3 diffuse = light.diffuse * Id;

    // specular
    float Is =  pow(max(dot(halfwayDir, normal), 0.0), 256);
    vec3 specular = light.specular * Is;

    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 fragToLight = normalize(light.position - fragPos);
    float cosTheta = dot(fragToLight, normalize(-light.direction));    //cosinus of the angle between the direction vector and the fragToLight vector (should be higher than light.cosAngle)
    float epsilon = light.cosInerAngle - light.cosOuterAngle;
    float intensity = clamp((cosTheta - light.cosOuterAngle) / epsilon, 0.0, 1.0);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    //ambient 
    vec3 ambient = light.ambient;

    // diffuse
    float Id = max(dot(lightDir,normal), 0.0);
    vec3 diffuse = light.diffuse * Id;

    // specular
    float Ia =  pow(max(dot(halfwayDir, normal), 0.0), 256);
    vec3 specular = light.specular * Ia;

    diffuse  *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
    
}
