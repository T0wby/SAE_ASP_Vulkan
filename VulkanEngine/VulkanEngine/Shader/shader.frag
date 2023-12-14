#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    mat4 transform;
} push;

void main() {
    outColor = texture(texSampler, fragTexCoord) * vec4(fragColor, 1.0);
}