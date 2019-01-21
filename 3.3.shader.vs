#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float PosX;
uniform float PosY;

void main()
{
    gl_Position = vec4(aPos.x+PosX, aPos.y+PosY, aPos.z, 1.0);
    ourColor = aColor;
}