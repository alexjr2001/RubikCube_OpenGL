#pragma once
//Includes and global variables

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "shader_s.h"
#include "D:\OpenGL\GLFW_GLAD_GLUT_GLEW_cmake_project\src\CuboRubik\Cubito.h"
#include "D:\OpenGL\GLFW_GLAD_GLUT_GLEW_cmake_project\src\CuboRubik\Camera.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//_______--------------------------------------

Camera cam;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

char sideMove;
bool isMoving = false;
bool changeDirection = false;
bool solving = false;
bool breath = false;
bool rubiks = false;
bool rotar = false;
//----------------------------------------


void init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    return;
}

GLFWwindow* createWindow(const char* title) {
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    
    return window;
}

bool glad(){
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
       std::cout << "Failed to initialize GLAD" << std::endl;
       return -1;
   }
    return 1;
}


//TEXTURE
void load_create_texture(unsigned int &texture, const char* title, bool flip=0) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void menu() {
    cout << "\t\tWELCOME TO RUBIK CUBE\n";
    cout << "Keys to move:" << "\n\t - MOUSE -> CAMERA" << "\n\t - UP arrow -> UP(U)"<< "\n\t - DOWN arrow -> DOWN(D)";
    cout << "\n\t - LEFT arrow -> LEFT(L)" << "\n\t - RIGHT arrow -> RIGHT(R)";
    cout << "\n\t - F -> FRONT(F)" << "\n\t - B -> BACK(B)"<< "\n\t - (Right) SHIFT -> CHANGE TO CLOCKWISE OR TO ANTICLOCKWISE";
    cout << "\nAnimation: " << "\n\t - R -> BREATH(START ANIMATION)" << "\n\t - S -> SOLVE (SMALL CUBE WILL FALL)";
    cout << "\nSHUFFLE: ";

    return;
}

void manager() {
    init();
    GLFWwindow* window = createWindow("Cubo Rubik");
    if (window == NULL || !glad()) return;  //return -1

    //Camera
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    Shader ourShader("shader.vs", "shader.fs");

    menu();

    Rubik rubik(window, &ourShader, 0.2f);

    Rubik newRubiks[4] = { Rubik(window, &ourShader, 0.05f),Rubik(window, &ourShader, 0.05f) ,Rubik(window, &ourShader, 0.05f) ,Rubik(window, &ourShader, 0.05f) };
    Rubik* rubik2=nullptr;
    int numRubik = 0; 


    while (!glfwWindowShouldClose(window))
    {
        cam.updateFrame();
        glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(cam.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(cam.cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cam.cameraUp);
        ourShader.setMat4("view", view);

        if (solving == true) {
            Rubik* rubikMoving = &rubik;
            Rubik* rubikStatic = nullptr;
            if (rubiks) {
                rubikMoving = rubik2;
                rubikStatic = &rubik;
            }
            rubikMoving->setSolve();
            
            solving = false;
            if (rubikStatic) {
                rubikMoving->vibrate();
                rubikStatic->magnet();
                rubiks = false;
            }
        }

        else if (breath) {
            rubik.expand();
            breath = false;
            rubik2 = &newRubiks[numRubik];
            numRubik++;
            rubik2->brothers = rubik.brothers;
            rubik2->brothers.push_back(&rubik);
            rubik.brothers.push_back(rubik2);
            rubiks = true;
            rotar = true;
        }
        else if (rotar) {
            if (isMoving == false && changeDirection) {
                changeDirection = false;
                rubik2->degrees *= -1;
            }
            if (isMoving) {
                rubik2->fillShuffle(sideMove);
                rubik2->move(sideMove);
                isMoving = false;
                for (int i = 0; rubik2->degrees == 1.0f && i < 2; i++) {
                    rubik2->updateParts(sideMove);
                }
                rubik2->updateParts(sideMove);
            }
            rubik2->twist();
            rubik2->timesTwist++;
            if (rubik2->timesTwist == 40) rotar = false;
        }

        else if (isMoving == false && changeDirection) {
            changeDirection = false;
            if (rubiks) rubik2->degrees *= -1;
            else rubik.degrees *= -1;
        }

        else if (isMoving == true) {
            Rubik* rubikMoving = &rubik;
            Rubik* rubikStatic = nullptr;
            if (rubiks) {
                rubikMoving = rubik2;
                rubikStatic = &rubik;
            }
            rubikMoving->fillShuffle(sideMove);
            rubikMoving->move(sideMove);
            isMoving = false;
            for (int i = 0; rubikMoving->degrees == 1.0f && i < 2; i++) {
                rubikMoving->updateParts(sideMove);
            }
            rubikMoving->updateParts(sideMove);
        }
        else {
            rubik.draw();
        }

    }
    rubik.deleteBuffers();
    //rubik2.deleteBuffers();
    for (int i = 0; i < 4; i++)
    {
        newRubiks[i].deleteBuffers();
    }
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 0.2;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.cameraPos += cameraSpeed * cam.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.cameraPos -= cameraSpeed * cam.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.cameraPos -= glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.cameraPos += glm::normalize(glm::cross(cam.cameraFront, cam.cameraUp)) * cameraSpeed;
    if (!isMoving) {
        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            changeDirection = true;
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            isMoving = true;
            sideMove = 'U';
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            isMoving = true;
            sideMove = 'L';
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            isMoving = true;
            sideMove = 'R';
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            isMoving = true;
            sideMove = 'D';
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            isMoving = true;
            sideMove = 'F';
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            isMoving = true;
            sideMove = 'B';
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !rotar) {
            solving = true;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            breath = true;
        }
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (cam.firstMouse)
    {
        cam.lastX = xpos;
        cam.lastY = ypos;
        cam.firstMouse = false;
    }

    float xoffset = xpos - cam.lastX;
    float yoffset = cam.lastY - ypos; // reversed since y-coordinates go from bottom to top
    cam.lastX = xpos;
    cam.lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    if (xoffset > 0 && cam.cameraPos.x < 5) cam.cameraPos.x += xoffset;
    else if (xoffset < 0 && cam.cameraPos.x > -5) cam.cameraPos.x += xoffset;

    if (yoffset > 0 && cam.cameraPos.y < 5) cam.cameraPos.y += yoffset;
    else if (yoffset < 0 && cam.cameraPos.y > -5) cam.cameraPos.y += yoffset;

    cam.yaw += xoffset;
    cam.pitch += yoffset;

    if (cam.pitch > 89.0f)
        cam.pitch = 89.0f;
    if (cam.pitch < -89.0f)
        cam.pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    front.y = sin(glm::radians(cam.pitch));
    front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.fov -= (float)yoffset;
    if (cam.fov < 1.0f)
        cam.fov = 1.0f;
    if (cam.fov > 45.0f)
        cam.fov = 45.0f;
}

