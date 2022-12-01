//
//  main.cpp
//  Triangles
//
//  Created by 이진희 on 2022/10/14.
//

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "toys.h"
#include <glm/glm.hpp>

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

    GLFWwindow* window = glfwCreateWindow( 640, 480, "Three Triangles", NULL, NULL );
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


using namespace std;
using namespace glm;

void init() {
    program.loadShaders("/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/Triangles/Triangles/shader.vert",
                        "/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/Triangles/Triangles/shader.frag");
    
    vector<vec3> vertices = {{0, 1, 0}, {-0.5, 0, 0}, {0.5, 0, 0}, {-1, -1, 0}, {0, -1, 0}, {1, -1, 0}};
    vector<u16vec3> elements = {{0, 1, 2}, {1, 3, 4}, {2, 4, 5}};
    
    glGenBuffers(1, &vertexBuffer); // 버퍼를 만든다.
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // 이 버퍼를 사용하겠다~
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW); // 데이터를 버퍼로 옮긴다.
    
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glEnableVertexAttribArray(0); // vertex 마다의 속성을 enable 시킨다.
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    
    glGenBuffers(1, &elementBuffer); // EBO는 vertex에 인덱스를 부여하여 렌더링시 재사용할 수 있게 해준다.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16vec3) * elements.size(), elements.data(), GL_STATIC_DRAW);
}

void render( GLFWwindow* window ) {
    int width, height;
    glfwGetFramebufferSize( window, &width, &height );
    
    glViewport( 0, 0, width, height );
    glClearColor( 1, 1, 1, 0 ); // 흰색으로 지우기
    glClear( GL_COLOR_BUFFER_BIT );
   
    glUseProgram(program.programID);
    
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, 0); // 점이 6개처럼 보이지만, 실제로 렌더링할 요소는 9개이므로 9를 매개변수로 준다.
    glfwSwapBuffers( window );
}
