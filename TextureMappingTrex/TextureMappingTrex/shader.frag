#version 150 core

out vec4 out_Color;

uniform vec4 color;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float shininess;
uniform vec3 cameraPosition;
uniform vec3 ambientLight;
uniform sampler2D diffTex;
uniform sampler2D bumpTex;

in vec3 normal;
in vec3 worldPosition;
in vec2 texCoords;

mat3 getTBN(vec3 N) {
    vec3 Q1 = dFdx(worldPosition), Q2 = dFdy(worldPosition);
    vec2 st1 = dFdx(texCoords), st2 = dFdy(texCoords);
    float D = st1.s * st2.t - st1.t * st2.s;
    return mat3( normalize( (Q1 * st2.t - Q2 * st1.t) * D ), normalize( (-Q1 * st2.s + Q2 * st1.s) * D), N);
}

void main(void)
{
    vec3 l = lightPosition - worldPosition;
    vec3 L = normalize( l );
    vec3 N = normalize( normal );
    
    mat3 tbn = getTBN(N);
    float dBdu = texture(bumpTex, texCoords + vec2(0.00001, 0)).r - texture(bumpTex, texCoords - vec2(0.00001, 0)).r;
    float dBdv = texture(bumpTex, texCoords + vec2(0, 0.00001)).r - texture(bumpTex, texCoords - vec2(0, 0.00001)).r;
    
    N = normalize( N - dBdu * tbn[0] * 100 - dBdv * tbn[1] * 100 );
    
    vec3 V = normalize( cameraPosition - worldPosition );
    vec3 R = 2 * dot(L, N) * N - L;
    vec3 I = lightColor / dot(l, l); // 들어오는 빛의 밝기
    vec4 color4 = texture(diffTex, texCoords);
    vec3 c = color4.rgb * max(0, dot( L, N )) * I + color4.rgb * ambientLight;
    c += pow( max(0, dot(R, V)), 100 ) * I; // shininess가 크면 클수록 빛이 뾰족하게 반사된다. specular color는 도체가 아니면 전부 1이기 때문에 곱하는 것을 생략하였다.
	out_Color = vec4( pow(c, vec3(1/2.2)), 1 );
}
