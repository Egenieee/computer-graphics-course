#version 150 core

out vec4 out_Color;

uniform vec4 color;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float shininess;
uniform vec3 cameraPosition;
uniform vec3 ambientLight;

in vec3 normal;
in vec3 worldPosition;

void main(void)
{
    vec3 l = lightPosition - worldPosition;
    vec3 L = normalize( l );
    vec3 N = normalize( normal );
    vec3 V = normalize( cameraPosition - worldPosition );
    vec3 R = 2 * dot(L, N) * N - L;
    vec3 I = lightColor / dot(l, l); // 들어오는 빛의 밝기
    vec3 c = color.rgb * max(0, dot( L, N )) * I + color.rgb * ambientLight;
    c += pow( max(0, dot(R, V)), 100 ) * I; // shininess가 크면 클수록 빛이 뾰족하게 반사된다. specular color는 도체가 아니면 전부 1이기 때문에 곱하는 것을 생략하였다.
	out_Color = vec4( pow(c, vec3(1/2.2)), 1 );
}
