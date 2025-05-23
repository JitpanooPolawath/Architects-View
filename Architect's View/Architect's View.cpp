// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// classes
#include "../Architect's View/shader.h"
#include "../Architect's View/camera.h"
#include "../Architect's View/objects.h"
#include "../Architect's View/stack.h"

// Basic include
#include <iostream>
#include <filesystem> 

// Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
// Camera attribute
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
camera testCam(cameraPos, cameraFront, cameraUp);

// Lighting
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);


// Resizing window frame
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process Input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = 0.01f * deltaTime;
    glm::vec3 cameraPos = testCam.cameraPos;
    glm::vec3 cameraFront = testCam.cameraFront;
    glm::vec3 cameraUp = testCam.cameraUp;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * (cameraFront);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * (cameraFront);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    testCam.cameraPos = cameraPos;
}

// Process mouse movement
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (testCam.firstMouse)
    {
        testCam.lastX = xpos;
        testCam.lastY = ypos;
        testCam.firstMouse = false;
    }

    float xoffset = xpos - testCam.lastX;
    float yoffset = testCam.lastY - ypos; // reversed since y-coordinates go from bottom to top
    testCam.lastX = xpos;
    testCam.lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    testCam.yaw += xoffset;
    testCam.pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (testCam.pitch > 89.0f)
        testCam.pitch = 89.0f;
    if (testCam.pitch < -89.0f)
        testCam.pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(testCam.yaw)) * cos(glm::radians(testCam.pitch));
    front.y = sin(glm::radians(testCam.pitch));
    front.z = sin(glm::radians(testCam.yaw)) * cos(glm::radians(testCam.pitch));
    testCam.cameraFront = glm::normalize(front);
}

// Transformation
// ----------------
// Translate 
glm::mat4 translate(glm::mat4 model, float x, float y, float z) {
    return glm::translate(model, glm::vec3(x, y, z));
}
// Scale
glm::mat4 scale(glm::mat4 model, float x, float y, float z) {
    return glm::scale(model, glm::vec3(x, y, z));
}
// Rotate
glm::mat4 rotate(glm::mat4 model, float degree, float x, float y, float z) {
    return glm::rotate(model, degree, glm::vec3(x,y,z));
}

void shaderParam(Shader shader, glm::vec3 color, glm::vec3 lightColor, int value) {
    shader.setColor(color);
    shader.setLightColor(lightColor);
    shader.setInt("ID", value);
}



int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initializing glad 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    if (std::filesystem::exists("vertex.vs") || std::filesystem::exists("fragment.fs")) {

    }
    else {
        std::cerr << "File does not exist" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    // Create shader object
    Shader lightShader("vertex.vs", "fragment.fs");
    // Create cube object
    Cube testCube;

    // Rendering loop
    while (!glfwWindowShouldClose(window)) {

        MatrixStack MS;
        
        // Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view;
        view = glm::lookAt(testCam.cameraPos, testCam.cameraPos + testCam.cameraFront, testCam.cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        lightShader.use();
        lightShader.setMV(model, view);
        lightShader.setProj(projection);
        lightShader.setViewPos(testCam.cameraPos);

        // Scene
        MS.push(model);
        
        model = translate(model, 0.0f, 0.0f, 0.0f);
        model = scale(model, 5.0f, 5.0f, 1.0f);
        shaderParam(lightShader, glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0);
        lightShader.setMV(model, view);
        testCube.drawCube();
        
        model = MS.pop();




        // ------------------------------------------------------------------------------
        // Lighting Cube

        MS.push(model);

        //model = rotate(model, lastFrame * 1.0f, 0, 1, 0);
        model = translate(model,0.0f, 0.0f, -2.0f);
        model = scale(model, 0.2f, 0.2f, 0.2f);

        shaderParam(lightShader, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1);
        lightShader.setMV(model, view);
        // Lighting position
        lightShader.setLightPos(glm::vec3(model[3]));
        testCube.drawCube();
        
        model = MS.pop();

        // update window
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    
    testCube.deleteBuffVer();
    glfwTerminate();
    return 0;
}