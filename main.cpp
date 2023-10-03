//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool fanRotationEnabled = false;
bool openBookshelf = false;
bool TVoff = false;
bool ss1 , ss2 , ss3;

// camera
Camera camera(glm::vec3(2.0f, 1.5f, 3.0f));
float lastX = SCR_WIDTH;
float lastY = SCR_HEIGHT;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 0.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 transformation(float transform_x, float transform_y, float transform_z, float rotate_x, float rotate_y,
    float rotate_z, float scale_x, float scale_y, float scale_z) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(transform_x, transform_y, transform_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_x, scale_y, scale_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    return model;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
        0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
        0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
        0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
        0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
        0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    };*/
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    float bed_vertices[] = {
          0.0f, 0.0f, 0.0f,0.1f, 0.0f, 0.0f,
          0.5f, 0.0f, 0.0f,0.1f, 0.0f, 0.0f,
          0.5f, 0.5f, 0.0f, 0.1f, 0.0f, 0.0f,
          0.0f, 0.5f, 0.0f, 0.1f, 0.0f, 0.0f,

          0.5f, 0.0f, 0.0f,0.1f, 0.0f, 0.0f,
          0.5f, 0.5f, 0.0f, 0.1f, 0.0f, 0.0f,
          0.5f, 0.0f, 0.5f, 0.1f, 0.0f, 0.0f,
          0.5f, 0.5f, 0.5f, 0.1f, 0.0f, 0.0f,

          0.0f, 0.0f, 0.5f, 0.1f, 0.0f, 0.0f,
          0.5f, 0.0f, 0.5f, 0.1f, 0.0f, 0.0f,
          0.5f, 0.5f, 0.5f, 0.1f, 0.0f, 0.0f,
          0.0f, 0.5f, 0.5f, 0.1f, 0.0f, 0.0f,

          0.0f, 0.0f, 0.5f,0.1f, 0.0f, 0.0f,
          0.0f, 0.5f, 0.5f, 0.1f, 0.0f, 0.0f,
          0.0f, 0.5f, 0.0f, 0.1f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f,

          0.5f, 0.5f, 0.5f, 0.1f, 0.0f, 0.0f,
          0.5f, 0.5f, 0.0f, 0.1f, 0.0f, 0.0f,
          0.0f, 0.5f, 0.0f, 0.1f, 0.0f, 0.0f,
          0.0f, 0.5f, 0.5f, 0.1f, 0.0f, 0.0f,

          0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f,
          0.5f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f,
          0.5f, 0.0f, 0.5f,0.1f, 0.0f, 0.0f,
          0.0f, 0.0f, 0.5f, 0.1f, 0.0f, 0.0f
    };
    float pillow_vertices[] = {
        0.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.5f,1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f,1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f
    };

    float sheet_vertices[] = {
        0.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,

        0.5f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f,0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f,0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f,0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f
    };


    float floor[] = {
        0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,

        0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

        0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

        0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,

        0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

        0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
        0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
        0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f
    };
    //0.92f, 0.91f, 0.83f,
    float wall1[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.925f, 0.702f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.925f, 0.702f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.925f, 0.702f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.925f, 0.702f,

        0.5f, 0.0f, 0.0f, 1.0f, 0.925f, 0.702f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.925f, 0.702f,
        0.5f, 0.0f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.5f, 0.5f, 0.5f,  1.0f, 0.925f, 0.702f,

        0.0f, 0.0f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.5f, 0.0f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.5f, 0.5f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.0f, 0.5f, 0.5f,  1.0f, 0.925f, 0.702f,

        0.0f, 0.0f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.0f, 0.5f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.0f, 0.5f, 0.0f,  1.0f, 0.925f, 0.702f,
        0.0f, 0.0f, 0.0f,  1.0f, 0.925f, 0.702f,

        0.5f, 0.5f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.925f, 0.702f,
        0.0f, 0.5f, 0.0f,  1.0f, 0.925f, 0.702f,
        0.0f, 0.5f, 0.5f,  1.0f, 0.925f, 0.702f,

        0.0f, 0.0f, 0.0f,  1.0f, 0.925f, 0.702f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.925f, 0.702f,
        0.5f, 0.0f, 0.5f,  1.0f, 0.925f, 0.702f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.925f, 0.702f
    };

    //0.99f, 0.84f, 0.70f,
    float wall2[] = {
        0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,

        0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

        0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

        0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,

        0.5f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.5f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.5f, 0.5f, 0.99f, 0.84f, 0.70f,

        0.0f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.0f, 0.0f, 0.99f, 0.84f, 0.70f,
        0.5f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f,
        0.0f, 0.0f, 0.5f, 0.99f, 0.84f, 0.70f
    };



    float ceiling[] = {
        0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,

        0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

        0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

        0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,

        0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

        0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
        0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
        0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f
    };






    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };*/
    /*float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };*/
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/
    unsigned int VBO1, VAO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bed_vertices), bed_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    unsigned int VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pillow_vertices), pillow_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);

        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;



        
        //left wall
        glm::mat4 scaleMatrix_wall = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 modelLeftWall = transformation(-0.80f, -0.30f, -1.0f, 0.0f, 90.0f, 0.0f, -14.0f, 4.8f, 0.0f);
        ourShader.setMat4("model", modelLeftWall * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(1.0f, 0.7f, 0.7f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Right wall
        glm::mat4 modelRightWall = transformation(2.28 * 2.80f, -0.30f, 6.0f, 0.0f, 90.0f, 0.0f, 14.0f, 4.8f, 0.0f);
        ourShader.setMat4("model", modelRightWall * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(1.0f, 0.7f, 0.7f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        // Front wall
        glm::mat4 modelFrontWall = transformation(-0.80f, -0.30f, -1.0f, 0.0f, 0.0f, 0.0f, 14.4f, 4.8f, 0.0f);
        ourShader.setMat4("model", modelFrontWall * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(1.0f, 0.7f, 0.7f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // back wall
        glm::mat4 modelBackWall = transformation(-0.80f, -0.30f, 5.0f, 0.0f, 0.0f, 0.0f, 14.4f, 4.8f, 0.0f);
        ourShader.setMat4("model", modelBackWall * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(1.0f, 0.7f, 0.7f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //// Top wall
        glm::mat4 modelTopWall = transformation(-0.80f, 2.0f, -1.0f, 90.0f, 0.0f, 0.0f, 14.3f, 14.0f, 0.0f);
        ourShader.setMat4("model", modelTopWall * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //// Bottom wall

        float x_trans = -0.8f;
        for (int i = 0; i < 10; i++)
        {
            float z_trans = -1.0f;
            for (int it = 0; it < 10; it++)
            {
                glm::mat4 tile1 = transformation(x_trans, -0.30f, z_trans, 90.0f, 0.0f, 0.0f, 1.5f, 1.5f, 0.0f);
                ourShader.setMat4("model", tile1 * scaleMatrix_wall);
                ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
                glBindVertexArray(VAO1);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

                z_trans += 0.77f;
            }
            x_trans += 0.77f;
        }
        

        //TV

        float a, b, c,x;
        
        if (!TVoff)
        {
            a = 0.0f;
            b = 0.0f;
            c = 0.0f;
            x = -0.97;
        }
        else
        {
            x = -0.99;
            if (ss1)
            {
                a = 1.0f;
                b = 1.0f;
                c = 1.0f;
                ss1 = false;
                ss2 = true;
            }
            else if (ss2)
            {
                a = 0.0f;
                b = 1.0f;
                c = 0.0f;
                ss2 = false;
                ss3 = true;
            }
            else if (ss3)
            {
                a = 1.0f;
                b = 0.0f;
                c = 0.0f;
                ss3 = false;
                ss1 = true;
            }

            
        }

        model = transformation(1.80f, 0.60f, x, 0.0f, 0.0f, 0.0f, 4.55f, 2.15f, 0.0f);
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(a, b, c, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //white
        model = transformation(1.80f, 0.60f, -0.99f, 0.0f, 0.0f, 0.0f, 4.5f, 2.1f, 0.0f);
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //fan

        currentFrame = static_cast<float>(glfwGetTime());
        float rotationSpeed = 200000.0f;
        float fanRotationAngle = 0.0f;
        if (fanRotationEnabled)
        {
            fanRotationAngle = glm::radians(rotationSpeed * currentFrame);
        }
        for (int i = 0; i < 4; ++i) {
            float rotateAngle = i * 90.0f + fanRotationAngle; // Update rotation angle
            //white
            glm::mat4 modelBlade = transformation(2.375f, 1.87f, 2.50f, 90.0f, 0.0f, rotateAngle, 2.50f, 0.5f, 0.03f);
            ourShader.setMat4("model", modelBlade);
            ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            glBindVertexArray(VAO1);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //black
            glm::mat4 modelBlade2 = transformation(2.375f, 1.86f, 2.50f, 90.0f, 0.0f, rotateAngle, 2.50f, 0.47f, 0.03f);
            ourShader.setMat4("model", modelBlade2);
            ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glBindVertexArray(VAO1);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        
        //sofa
        glm::mat4 sofa = transformation(0.78f, -0.3f, 4.6f, 0.0f, 0.0f, 0.0f, 5.2f, 1.5f, 0.6f);
        ourShader.setMat4("model", sofa * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.80f, 0.65f, 0.5f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 sofaSeat = transformation(0.80f, -0.3f, 4.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.8f, 1.65f);
        ourShader.setMat4("model", sofaSeat * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.8f, 0.7f, 0.6f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glm::mat4 sofaHandle = transformation(0.75f, -0.3f, 4.1f, 0.0f, 0.0f, 0.0f, 0.5f, 1.2f, 1.65f);
        ourShader.setMat4("model", sofaHandle * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.87f, 0.72f, 0.53f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 sofaHandle2 = transformation(3.15f, -0.3f, 4.1f, 0.0f, 0.0f, 0.0f, 0.5f, 1.2f, 1.65f);
        ourShader.setMat4("model", sofaHandle2 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.87f, 0.72f, 0.53f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //table
        glm::mat4 table = transformation(1.00f, 0.20f, 3.0f, 0.0f, 0.0f, 0.0f, 4.0f, 0.2f, 1.6f);
        ourShader.setMat4("model", table * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //table leg
        glm::mat4 tablel = transformation(1.00f, -0.3f, 3.0f, 0.0f, 0.0f, 0.0f, 0.3f, 1.0f, 0.3f);
        ourShader.setMat4("model", tablel * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 tablel2 = transformation(1.00f, -0.3f, 3.65f, 0.0f, 0.0f, 0.0f, 0.3f, 1.0f, 0.3f);
        ourShader.setMat4("model", tablel2 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 tablel3 = transformation(2.8f, -0.3f, 3.65f, 0.0f, 0.0f, 0.0f, 0.3f, 1.0f, 0.3f);
        ourShader.setMat4("model", tablel3 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 tablel4 = transformation(2.8f, -0.3f, 3.0f, 0.0f, 0.0f, 0.0f, 0.3f, 1.0f, 0.3f);
        ourShader.setMat4("model", tablel4 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        //Clock
        glm::mat4 WallClock = transformation(6.10f, 1.00f, 3.0f, 0.0f, 90.0f, 0.0f, 1.5f, 1.5f, 0.3f);
        ourShader.setMat4("model", WallClock* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 time12 = transformation(6.09f, 1.67f, 2.65f, 0.0f, 90.0f, 0.0f, 0.1f, 0.1f, 0.3f);
        ourShader.setMat4("model", time12 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 time6 = transformation(6.09f, 1.05f, 2.65f, 0.0f, 90.0f, 0.0f, 0.1f, 0.1f, 0.3f);
        ourShader.setMat4("model", time6 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 time3 = transformation(6.09f, 1.36f, 2.96f, 0.0f, 90.0f, 0.0f, 0.1f, 0.1f, 0.3f);
        ourShader.setMat4("model", time3 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 time9 = transformation(6.09f, 1.36f, 2.34f, 0.0f, 90.0f, 0.0f, 0.1f, 0.1f, 0.3f);
        ourShader.setMat4("model", time9 * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        float clockRotationAngle = glm::radians(600 * currentFrame);
        glm::mat4 min = transformation(6.09f, 1.38f, 2.637f, 0.0f, 90.0f, clockRotationAngle, 0.05f, 0.55f, 0.05f);
        ourShader.setMat4("model", min* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        float clockRotationAngle2 = glm::radians(100 * currentFrame);
        glm::mat4 hour = transformation(6.09f, 1.38f, 2.637f, 0.0f, 90.0f, clockRotationAngle2, 0.05f, 0.50f, 0.05f);
        ourShader.setMat4("model", hour* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        // Book shelf
        glm::mat4 rightWood = transformation(1.0f, -0.30f, 0.0f, 0.0f, 90.0f, 0.0f, 2.0f, 3.8f, 0.15f);
        ourShader.setMat4("model", rightWood* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.53f, 0.29f, 0.03f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 leftWood = transformation(0.0f, -0.30f, 0.0f, 0.0f, 90.0f, 0.0f, 2.0f, 3.8f, 0.15f);
        ourShader.setMat4("model", leftWood * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.53f, 0.29f, 0.03f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 backWood = transformation(-0.00f, -0.30f, -0.9f, 0.0f, 0.0f, 0.0f, 2.0f, 3.8f, 0.0f);
        ourShader.setMat4("model", backWood * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.38f, 0.22f, 0.07f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 bottomWood = transformation(0.00f, -0.29f, -1.0f, 90.0f, 0.0f, 0.0f, 2.1f, 2.0f, 1.3f);
        ourShader.setMat4("model", bottomWood * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.36f, 0.18f, 0.07f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 topWood = transformation(0.00f, 1.6f, -1.04f, 90.0f, 0.0f, 0.0f, 2.1f, 2.0f, 0.15f);
        ourShader.setMat4("model", topWood * scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.36f, 0.18f, 0.07f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        float openAngle;
        if (!openBookshelf)
            openAngle = 0.0f;
        else
            openAngle = -120.0f;

        glm::mat4 frontWood = transformation(-0.00f, -0.30f, 0.02f, 0.0f, openAngle, 0.0f, 1.0f, 3.8f, 0.0f);
        ourShader.setMat4("model", frontWood* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.53f, 0.29f, 0.03f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 frontWood2 = transformation(1.05f, -0.30f, 0.02f, 0.0f, 180-openAngle, 0.0f, 1.0f, 3.8f, 0.0f);
        ourShader.setMat4("model", frontWood2* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.53f, 0.29f, 0.03f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glm::mat4 s1 = transformation(0.00f, 1.0f, -1.04f, 90.0f, 0.0f, 0.0f, 2.1f, 2.0f, 0.15f);
        ourShader.setMat4("model", s1* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.36f, 0.18f, 0.07f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 s2 = transformation(0.00f ,0.4f, -1.04f, 90.0f, 0.0f, 0.0f, 2.1f, 2.0f, 0.15f);
        ourShader.setMat4("model", s2* scaleMatrix_wall);
        ourShader.setVec4("color", glm::vec4(0.36f, 0.18f, 0.07f, 1.0f));
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);


    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && fanRotationEnabled == false)
    {
        fanRotationEnabled = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && fanRotationEnabled == true)
    {
        fanRotationEnabled = false;
    }        
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && openBookshelf == false)
    {
        openBookshelf = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && openBookshelf == true)
    {
        openBookshelf = false;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && TVoff == false)
    {
        TVoff = true;
        ss1 = true, ss2 = false, ss3 = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && TVoff == true)
    {
        TVoff = false;
    }
   

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(PITCH_D, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(PITCH_U, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ROLL_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ROLL_L, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
