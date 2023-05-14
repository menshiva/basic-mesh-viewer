out vec4 color;

uniform vec3 u_Color;

void main() {
    color = vec4(u_Color.rgb, 1.0f);
}
