#version 410 core

layout(location=0) in vec3 in_Position;

uniform mat4 transform;

void main(void)
{
    vec4 p = vec4(in_Position.xyz, 1);
    p = transform * p;
    gl_Position = p;
}
