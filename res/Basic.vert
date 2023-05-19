in vec2 position;

uniform float u_AspectRatio;
uniform vec2 u_CenterOffset;

void main() {
    gl_Position = vec4(position * vec2(u_AspectRatio, 1.0f) + u_CenterOffset, 0.0f, 1.0f);
}
