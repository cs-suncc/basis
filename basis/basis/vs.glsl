
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 txCoord;

out vec3 OurColor;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    OurColor = vColor;
    TexCoord = txCoord;
}