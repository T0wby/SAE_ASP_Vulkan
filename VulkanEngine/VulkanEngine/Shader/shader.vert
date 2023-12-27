#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
} ubo;

layout(push_constant) uniform Push {
    mat4 transform;
} push;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 fragTexCoord;

void main() {
    vec4 positionWorld = push.transform * vec4(inPosition, 1.0);
    gl_Position = (ubo.proj * ubo.view * ubo.model) * positionWorld;
    
    mat3 normalMatrix = mat3(transpose(inverse(ubo.model)));
    vec3 normalWorldSpace = normalize(normalMatrix * inNormal);
    
    vec3 directionToLight = ubo.lightPosition - positionWorld.xyz;
    float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
    
    vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
    vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 diffuseLight = lightColor *  max(dot(normalWorldSpace, normalize(directionToLight)), 0);
    
    
    fragColor = (diffuseLight + ambientLight) * inColor;
    fragTexCoord = inTexCoord;
    Normal = mat3(transpose(inverse(push.transform))) * inNormal;
}