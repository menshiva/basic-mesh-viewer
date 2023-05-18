in vec4 position;

uniform float u_AspectRatio;

void main() {
    gl_Position = vec4(position.x * u_AspectRatio, position.yzw);
}
