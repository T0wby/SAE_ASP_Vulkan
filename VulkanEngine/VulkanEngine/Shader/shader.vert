#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightDirection;
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

const float AMBIENT = 0.02;

void main() {
    gl_Position = (ubo.proj * ubo.view * ubo.model) * push.transform * vec4(inPosition, 1.0);
    
    //vec3 normalWorldSpace = normalize(mat3(ubo.model) * inNormal);
    mat3 normalMatrix = mat3(transpose(inverse(ubo.model)));
    vec3 normalWorldSpace = normalize(normalMatrix * inNormal);
    float lightIntensity = AMBIENT + max(dot(normalWorldSpace, ubo.lightDirection), 0);
    
    fragColor = lightIntensity * inColor;
    fragTexCoord = inTexCoord;
    Normal = mat3(transpose(inverse(push.transform))) * inNormal;
}