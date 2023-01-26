#version 330 core
out vec4 FragColor;

in vec4 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 camPos;
uniform int nbDirLights;

uniform bool drawLine;
uniform bool enableText;
uniform bool doToon;
uniform float alpha;
uniform float beta;
uniform vec3 coldColor;
uniform vec3 hotColor;

vec4 gooch_shading(vec4 m_color, float m_shine, vec3 l_direction, vec3 v_normal, vec3 c_direction);
vec4 toon_shading(vec4 m_color, vec4 l_diffuseColor, vec3 l_direction, vec3 v_normal, vec3 c_direction);

// Directionnal Lights
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_DIR_LIGHTS 1
uniform DirLight dirLights[NR_DIR_LIGHTS];

void main()
{
    if(!drawLine)
    {
        vec3 norm = normalize(Normal);
        vec3 fragPos = vec3(FragPos);
        vec3 view = normalize(camPos - fragPos);
        vec3 dirLight = -normalize(dirLights[0].direction);
        
        if(enableText)
            FragColor = texture(texture1, TexCoord);
        else
            FragColor = vec4(0,0,0,1);

        if(doToon)
            FragColor = toon_shading(FragColor, vec4(dirLights[0].diffuse,1.0), dirLight, norm, view);
        else
            FragColor = gooch_shading(FragColor, 100.f, dirLight, norm, view);
    }
    else
    {
        FragColor = vec4(0,0,0,0);
    }
};

vec4 toon_shading(vec4 m_color,vec4 l_diffuseColor, vec3 l_direction, vec3 v_normal,vec3 c_direction)
{
    float intensity = dot(l_direction,v_normal);
    vec4 toonColor;

    if (intensity > 0.95)
		toonColor = (l_diffuseColor * 1.0);
	else if (intensity > 0.6)
		toonColor = (l_diffuseColor * 0.8);
	else if (intensity > 0.30)
		toonColor = (l_diffuseColor * 0.5);
    else if (intensity > 0.15)
		toonColor = (l_diffuseColor * 0.35);
	else
		toonColor = (l_diffuseColor * 0.25);

    //Highlights
    vec3 reflectDir = reflect( -normalize(l_direction), normalize(v_normal));
    float specular = clamp( dot( normalize(c_direction), normalize(reflectDir)),0, 1);
    vec4 spec = vec4(1) * pow(specular, 100.f);

    if(enableText)
        return m_color * (toonColor + spec);
    else
        return (toonColor + spec);
}

vec4 gooch_shading(vec4 m_color, float m_shine, vec3 l_direction,vec3 v_normal, vec3 c_direction) 
{
    //diffuse
    float a = 0.4f;
    float b = 0.5f;

    vec3 kcold = coldColor + alpha * m_color.xyz;
    vec3 khot = hotColor + beta * m_color.xyz;

    float dotNormDir = dot(normalize(v_normal), normalize(l_direction));
    
    float it = ((1 + dotNormDir) / 2.0f);
    vec3 color = it * khot + (1-it) * kcold;
    
    //Highlights
    vec3 reflectDir = reflect( -normalize(l_direction), normalize(v_normal));
    float specular = clamp( dot( normalize(c_direction), normalize(reflectDir)),0, 1);
    
    vec4 spec = vec4(1) * pow(specular, m_shine);

    return vec4(color + spec.xyz, m_color.a);
}