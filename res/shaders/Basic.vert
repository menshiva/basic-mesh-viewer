layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec3 in_Color;

out vec3 vert_Color;

uniform float u_AspectRatio;
uniform vec2 u_CenterOffset;
uniform bool u_UseOverrideColor;
uniform vec3 u_OverrideColor;
uniform float u_Scale;

void main() {
    gl_Position = vec4((in_Position * vec2(u_AspectRatio, 1.0f) + u_CenterOffset) * u_Scale, 0.0f, 1.0f);
    vert_Color = u_UseOverrideColor ? u_OverrideColor : in_Color;
}
