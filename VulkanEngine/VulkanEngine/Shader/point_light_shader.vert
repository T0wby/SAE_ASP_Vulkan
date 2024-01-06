#version 450

const float ONE = 1.0;
const float TWO = 2.0;

const vec2 OFFSETS[6] = vec2[](
vec2(-TWO, -TWO),
vec2(-TWO, TWO),
vec2(TWO, -TWO),
vec2(TWO, -TWO),
vec2(-TWO, TWO),
vec2(TWO, TWO)
);

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
} ubo;

layout(location = 0) out vec2 fragOffset;

const float LIGHT_RADIUS = 0.1;

void main() {
    fragOffset = OFFSETS[gl_VertexIndex];
    vec3 cameraRightWorld = {ubo.view[0][0], ubo.view[1][0], ubo.view[2][0]};
    vec3 cameraUpWorld = {ubo.view[0][1], ubo.view[1][1], ubo.view[2][1]};

    vec3 positionWorld = ubo.lightPosition.xyz + LIGHT_RADIUS * fragOffset.x * cameraRightWorld + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;
    gl_Position = (ubo.proj * ubo.view) * vec4(positionWorld, 1.0);
}