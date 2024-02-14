#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
} ubo;

layout(location = 0) in vec2 fragOffset;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(ubo.lightColor.xyz, 1.0);
}