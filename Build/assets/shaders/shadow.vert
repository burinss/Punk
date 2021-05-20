#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 P_lightSpace;
uniform mat4 model ;

void main()
{
    vec4 pos = vec4(position, 1.0);
    gl_Position = P_lightSpace * model * pos;
}
