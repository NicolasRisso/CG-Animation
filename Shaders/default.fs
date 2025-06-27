#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    
    float constant;
    float linear;
    float quadratic;
};

#define MAX_LIGHTS 4

uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;

// Função para calcular o efeito metálico cobre-ouro
vec3 calculateMetallicEffect(vec2 texCoords, vec3 normal, vec3 viewDir) {
    // Base de cobre
    vec3 copperColor = vec3(0.95, 0.64, 0.54);
    
    // Base de ouro
    vec3 goldColor = vec3(1.0, 0.84, 0.0);
    
    // Padrão de mistura baseado nas coordenadas de textura e normal
    float pattern = sin(texCoords.x * 10.0) * sin(texCoords.y * 10.0) * 0.5 + 0.5;
    
    // Adicionar variação baseada no ângulo de visão para efeito metálico
    float fresnel = pow(1.0 - max(dot(normal, viewDir), 0.0), 5.0);
    
    // Misturar cores com base no padrão e efeito fresnel
    vec3 baseColor = mix(copperColor, goldColor, pattern);
    
    // Adicionar brilho metálico
    vec3 metallicColor = mix(baseColor, vec3(1.0), fresnel * 0.7);
    
    return metallicColor;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 color = texture(material.diffuse, TexCoords).rgb;

    vec3 ambient = 0.1 * color;
    
    vec3 result = ambient;
    
    // 5) para cada luz, some diffuse + specular
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        // direção da luz
        vec3 lightDir = normalize(lights[i].position - FragPos);
        // diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * color * lights[i].color;
        // specular (Blinn-Phong)
        vec3 halfway = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfway), 0.0), material.shininess);
        vec3 specular = spec * lights[i].color;
        // atenuação
        float dist = length(lights[i].position - FragPos);
        float atten = 1.0 / (lights[i].constant
                             + lights[i].linear * dist
                             + lights[i].quadratic * dist * dist);
        diffuse  *= atten;
        specular *= atten;
        result += diffuse + specular;
    }

    FragColor = vec4(result, 1.0); // Magenta
}
