#version 450
//#extension GL_KHR_vulkan_glsl : enabled

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform Push {
    vec3 offset;
    vec3 color;
} push;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;


layout(location = 1) out vec2 fragTexCoord;


void main() {
    gl_Position = (ubo.proj * ubo.view * ubo.model) * vec4(inPosition + push.offset, 1.0);
    fragTexCoord = inTexCoord;
}