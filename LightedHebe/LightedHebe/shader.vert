#version 410 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

uniform mat4 modelMat;
uniform mat4 viewingMat;
uniform mat4 projectionMat;

out vec3 normal;
out vec3 worldPosition;

void main(void)
{
    vec4 p = vec4( in_Position.xyz, 1 );
    p = projectionMat * viewingMat * modelMat * p;
    normal = normalize( modelMat * vec4( in_Normal, 0 )).xyz;
    worldPosition = vec3( modelMat * vec4( in_Position, 1 ));
    gl_Position = p;
}
