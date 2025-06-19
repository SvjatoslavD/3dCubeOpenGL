#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 ColorVal;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(ColorVal, 1.0);
}