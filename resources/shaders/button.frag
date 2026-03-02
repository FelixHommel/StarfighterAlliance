#version 330 core
out vec4 FragColor;

uniform vec3 spriteColor;

void main()
{
    FragColor = vec4(spriteColor, 1.f);
}
