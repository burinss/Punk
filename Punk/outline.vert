#version 450 core

layout (location = 0) in vec3 position;
layout (location = 0) out vec3 FragPos;

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
    vec4 pos = vec4(position, 1.0);
    FragPos = vec3(model * pos);
    gl_Position = viewProjection * model * pos;
}
