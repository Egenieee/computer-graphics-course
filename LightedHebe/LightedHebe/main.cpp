//
//  main.cpp
//  LightedHebe
//
//  Created by 이진희 on 2022/11/26.
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
void cursorPosCallBack( GLFWwindow* window, double xpos, double ypos );
void scrollCallBack( GLFWwindow* window, double xoffset, double yoffset);

Program program;

int main(void){
    if ( !glfwInit() )        exit(EXIT_FAILURE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    
    glfwWindowHint( GLFW_SAMPLES, 8 ); // anti-aliasing 경계선을 부드럽게 만들자
    GLFWwindow* window = glfwCreateWindow( 640, 480, "Lighted Hebe", NULL, NULL );
    glfwSetCursorPosCallback(window, cursorPosCallBack); // cursor의 Position이 바뀌면 나를 call해줘~
    glfwSetScrollCallback(window, scrollCallBack); // scroll해서 값이 바뀌면 나를 call해줘~
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
GLuint normalBuffer = 0; // normal을 전달하기 위해 만들었다.
GLuint vertexArray = 0;
GLuint elementBuffer = 0;

float cameraTheta = 0;
float cameraPhi = 0;
float cameraDistance = 5;
float fovy = 50*PI/180;

using namespace std;
using namespace glm;

vec3 lightPosition = vec3(3, 10, 5); // light의 위치를 set
vec3 lightColor = vec3(100); // 밝기
vec3 ambientLight = vec3(0.0);

void init() {
    loadJ3A("/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/LightedHebe/LightedHebe/Archive/hebe.j3a");
    program.loadShaders("/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/LightedHebe/LightedHebe/shader.vert",
                        "/Users/j2n2/Desktop/Ajou Univ./2022-1/CG/assignments/LightedHebe/LightedHebe/shader.frag");
    
    glGenBuffers(1, &vertexBuffer); // 버퍼를 만든다.
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // 이 버퍼를 사용하겠다~
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &normalBuffer); // 버퍼를 만든다.
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer); // 이 버퍼를 사용하겠다~
    glBufferData(GL_ARRAY_BUFFER, nVertices[0]*sizeof(vec3), normals[0], GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0); // vertex 마다의 속성을 enable 시킨다.
    glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glEnableVertexAttribArray(1); // vertex 마다의 속성을 enable 시킨다. 1번에다 보내주겠다.
    glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
    
    glGenBuffers(1, &elementBuffer); // EBO는 vertex에 인덱스를 부여하여 렌더링시 재사용할 수 있게 해준다.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0]*sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);
}

void cursorPosCallBack( GLFWwindow* window, double xpos, double ypos ) {
    static double lastX = 0;
    static double lastY = 0;
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
        double dx = xpos - lastX;
        double dy = ypos - lastY;
        
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        
        cameraTheta -= (dx/w) * PI;
        cameraPhi -= (dy/h) * PI;
        
        if( cameraPhi < -PI/2+0.01 ) {
            cameraPhi = -PI/2+0.01;
        }
        
        if( cameraPhi > PI/2-0.01 ) {
            cameraPhi = PI/2-0.01;
        }
        
        printf("%f %f\n", cameraTheta, cameraPhi);
    }
    lastX = xpos;
    lastY = ypos;
}

void scrollCallBack( GLFWwindow* window, double xoffset, double yoffset) {
    //cameraDistance = cameraDistance * pow(1.05, yoffset);
    fovy += yoffset/30;
    
    if ( fovy < 0 ) {
        fovy = 0;
    }
    
    if ( fovy > PI ) {
        fovy = PI;
    }
    
    printf("%f\n", fovy);
}

void render( GLFWwindow* window ) {
    int width, height;
    glfwGetFramebufferSize( window, &width, &height );
    
    glViewport( 0, 0, width, height );
    glClearColor( 0.623529, 0.623529, 0.372549, 0 ); // 카키색 배경
    glEnable( GL_DEPTH_TEST );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
   
    glUseProgram(program.programID); // 어떤 프로그램인지 결정한 다음에야 uniform 변수를 세팅할 수 있다.
    
    vec3 initialCameraPosition = vec3(0, 0, cameraDistance);
    mat4 cameraRotationMatrix1 = rotate(cameraPhi, vec3(1, 0, 0)); // *
    mat4 cameraRotationMatrix2 = rotate(cameraTheta, vec3(0, 1, 0));
    
    vec3 cameraPosition = cameraRotationMatrix2 * cameraRotationMatrix1 * vec4(initialCameraPosition, 1); // 알아서 뒤에 1 잘라서 type을 맞춰준다.
    
    mat4 viewingMatrix = lookAt(cameraPosition, vec3(0, 0, 0), vec3(0, 1, 0));
    mat4 projectionMatrix = perspective(fovy, width/(float)height, 0.01f, 1000.f);
    
    GLuint modelMatLocation = glGetUniformLocation(program.programID, "modelMat"); // vertex shader에 uniform으로 선언된 행렬의 Location을 얻어온다.
    glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, value_ptr(mat4(1))); // 만든 행렬을 uniform 행렬 변수로 전달
    
    GLuint viewingMatLocation = glGetUniformLocation(program.programID, "viewingMat");
    glUniformMatrix4fv(viewingMatLocation, 1, GL_FALSE, value_ptr(viewingMatrix));
    
    GLuint projectionMatLocation = glGetUniformLocation(program.programID, "projectionMat");
    glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
    
    GLuint colorLocation = glGetUniformLocation(program.programID, "color");
    glUniform4fv(colorLocation, 1, value_ptr(diffuseColor[0])); // 4차원 float 벡터를 보낸다. 파일에 정의되어 있는 값을 보내자.
    
    GLuint shininessLocation = glGetUniformLocation(program.programID, "shininess");
    glUniform1f(shininessLocation, shininess[0]); // shininess도 마찬가지로 파일에 정의되어 있는 값을 보내자.
    
    GLuint lightPositionLocation = glGetUniformLocation(program.programID, "lightPosition");
    glUniform3fv(lightPositionLocation, 1, value_ptr(lightPosition));
    
    GLuint ambientLightLocation = glGetUniformLocation(program.programID, "ambientLight");
    glUniform3fv(ambientLightLocation, 1, value_ptr(ambientLight));
    
    GLuint cameraPositionLocation = glGetUniformLocation(program.programID, "cameraPosition");
    glUniform3fv(cameraPositionLocation, 1, value_ptr(cameraPosition));
    
    GLuint lightColorLocation = glGetUniformLocation(program.programID, "lightColor");
    glUniform3fv(lightColorLocation, 1, value_ptr(lightColor));
    
    
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    
    glDrawElements(GL_TRIANGLES, nTriangles[0]*3, GL_UNSIGNED_INT, 0);
    
    glfwSwapBuffers( window );
}

