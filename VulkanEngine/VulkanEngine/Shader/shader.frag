#version 450

layout(binding = 1) uniform sampler2D texSampler;


layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
    vec3 offset;
    vec3 color;
} push;

void main() {
    outColor = texture(texSampler, fragTexCoord) * vec4(push.color, 1.0);
}