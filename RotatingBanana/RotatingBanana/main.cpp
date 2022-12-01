//
//  main.cpp
//  RotatingBanana
//
//  Created by 이진희 on 2022/11/13.
//

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "toys.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "j3a.hpp"

const float PI = 3.14159265358979f;

void render( GLFWwindow* window );
void init();

Program program;

int main(void){
    if ( !glfwInit() )        exit(EXIT_FAILURE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow* window = glfwCreateWindow( 640, 480, "Ratating Banana", NULL, NULL );
    glfwMakeContextCurrent( window );
    
    init();
    
    while ( !glfwWindowShouldClose( window ) ) {
      render( window );
      glfwPollEvents();
    }
    
    glfwDestroyWindow( window );
    glfwTerminate();
}

GLuint vertexBuffer = 0; // 0은 아직 안만들어졌다라는 의미
GLuint vertexArray = 0;
GLuint elementBuffer = 0;

float theta = 0;
float scaleFactor = 1.0;
bool isShrinking = true;


using namespace std;
using namespace glm;

void init() {
    loadJ3A("/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/RotatingBanana/RotatingBanana/archive/banana.j3a");
    program.loadShaders("/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/RotatingBanana/RotatingBanana/shader.vert",
                        "/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/RotatingBanana/RotatingBanana/shader.frag");
    
    glGenBuffers(1, &vertexBuffer); // 버퍼를 만든다.
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // 이 버퍼를 사용하겠다~
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), vertices[0], GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glEnableVertexAttribArray(0); // vertex 마다의 속성을 enable 시킨다.
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    
    glGenBuffers(1, &elementBuffer); // EBO는 vertex에 인덱스를 부여하여 렌더링시 재사용할 수 있게 해준다.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);
}

void render( GLFWwindow* window ) {
    int width, height;
    glfwGetFramebufferSize( window, &width, &height );
    
    glViewport( 0, 0, width, height );
    glClearColor( 0.623529, 0.623529, 0.372549, 0 ); // 카키색 배경
    glClear( GL_COLOR_BUFFER_BIT );
   
    glUseProgram(program.programID); // 어떤 프로그램인지 결정한 다음에야 uniform 변수를 세팅할 수 있다.
    
    GLuint colorLocation = glGetUniformLocation(program.programID, "color");
    glUniform4fv(colorLocation, 1, value_ptr(vec4(1, 1, 0, 1))); // 4차원 float 벡터를 보낸다.
    
    theta += 5.0f/180*PI;
    
    mat4 rotationMatrix = rotate(theta, vec3(0,1,0)); // y축 회전 matrix 생성
    
    GLuint transformLocation = glGetUniformLocation(program.programID, "transform"); // vertex shader에 uniform으로 선언된 행렬의 Location을 얻어온다.
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(rotationMatrix)); // 만든 행렬을 uniform 행렬 변수로 전달
    
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0); // 파일에서 읽은 삼각형 수 * 3 만큼 삼각형 그리기
    
    glfwSwapBuffers( window );
}
