#version 330 core
in vec3 OurColor;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D theTexture;

void main() {
    FragColor = texture(theTexture, -TexCoord); //* vec4(OurColor, 1.0);
}