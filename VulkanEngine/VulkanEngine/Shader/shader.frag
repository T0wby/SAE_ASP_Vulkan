#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
    vec3 camPos;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormalWorld;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragPosWorld;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    mat4 transform;
} push;

void main() {
    // Ambient
    vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    
    // Diffuse
    vec3 norm = normalize(fragNormalWorld);
    vec3 directionToLight = normalize(ubo.lightPosition - fragPosWorld);
    float diffImpact = max(dot(norm, directionToLight), 0.0);
    vec3 diffuse = vec3(1.0f, 1.0f, 1.0f) * diffImpact;
    
    // Specular
    vec3 viewDir = normalize(ubo.camPos - fragPosWorld); // Direction of our Camera to the current fragment
    vec3 reflectDir = reflect(-directionToLight, norm); // Direction of the reflected light
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(1.0f, 1.0f, 1.0f) * (spec * vec3(0.727811f, 0.626959f, 0.626959f));
    
    //float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

    //vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
    //vec3 diffuseLight = lightColor *  max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0);
    
    outColor = texture(texSampler, fragTexCoord) * vec4((diffuse + ambientLight + specular) * fragColor, 1.0);
}