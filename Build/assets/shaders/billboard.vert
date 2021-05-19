#version 450 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out    vec2 FragUV;


uniform mat4 viewProjection;
uniform mat4 model;
uniform vec2 billboardSize;
uniform vec3 billboardPosition;

void main()
{
    FragUV=uv;
    vec4 pos = vec4(billboardPosition, 1.0);
    //FragPos = vec3(model * pos);
    gl_Position = viewProjection * pos;
    gl_Position /= gl_Position.w;
    gl_Position.xy += position.xy * billboardSize;
}