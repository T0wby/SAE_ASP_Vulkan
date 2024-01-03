#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
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
    vec3 directionToLight = ubo.lightPosition - fragPosWorld;
    float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

    vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
    vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 diffuseLight = lightColor *  max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0);
    
    outColor = texture(texSampler, fragTexCoord) * vec4((diffuseLight + ambientLight) * fragColor, 1.0);
}