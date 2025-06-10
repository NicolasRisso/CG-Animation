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
    // Propriedades do material
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Calcular efeito metálico cobre-ouro
    vec3 metallicColor = calculateMetallicEffect(TexCoords, normal, viewDir);
    
    // Iluminação ambiente base
    vec3 ambient = vec3(0.1) * metallicColor;
    
    // Resultado final da iluminação
    vec3 result = ambient;
    
    // Calcular contribuição de cada luz
    for(int i = 0; i < MAX_LIGHTS; i++) {
        // Direção da luz
        vec3 lightDir = normalize(lights[i].position - FragPos);
        
        // Difuso
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * metallicColor;
        
        // Especular
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * lights[i].color;
        
        // Atenuação
        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + 
                            lights[i].quadratic * (distance * distance));
        
        // Aplicar atenuação
        diffuse *= attenuation;
        specular *= attenuation;
        
        // Adicionar à iluminação total
        result += diffuse + specular;
    }
    
    // Saída final
    FragColor = vec4(result, 1.0);
}
