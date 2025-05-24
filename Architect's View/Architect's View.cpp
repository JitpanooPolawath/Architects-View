// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// imGui
#include <imGui/imgui.h>
#include <imGui/imgui_impl_glfw.h>
#include <imGui/imgui_impl_opengl3.h>

// classes
#include "../Architect's View/shader.h"
#include "../Architect's View/camera.h"
#include "../Architect's View/objects.h"
#include "../Architect's View/stack.h"

// Basic include
#include <iostream>
#include <filesystem> 

// Screen size
int SCR_W = 1080;
int SCR_H = 900;

// Time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
// Camera attribute
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
camera testCam(cameraPos, cameraFront, cameraUp);

// Object
float ambO[] = { 0.0,0.0,0.0 };
float difO[] = { 0.0,0.0,0.0 };
float speO[] = { 0.0,0.0,0.0 };
float shininess = 0.0;

// Lighting
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 lightAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
float amb[] = { 0.0,0.0,0.0 };
float dif[] = { 0.0,0.0,0.0 };
float spe[] = { 0.0,0.0,0.0 };
float pos[] = { 0.0,0.0,-2.0 };
bool trackWithCam = false;


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

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        trackWithCam = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        trackWithCam = false;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        testCam.mouseMove = false;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        testCam.mouseMove = true;
        testCam.firstMouse = true;
    }

}

// Process mouse movement
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    if (testCam.mouseMove) {

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

void setColor(Shader shader, Cube cubic) {
    shader.setColor(cubic.shininess);
    shader.setID("ID", 0);
}

void setLight(Shader shader, glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
    shader.setLight(pos, ambient, diffuse, specular);
    shader.setFloat("light.constant", 1.0f);
    shader.setFloat("light.linear", 0.9f);
    shader.setFloat("light.quadratic", 0.032f);
    shader.setID("ID", 1);
}

void setImLight() {
    ImGui::Text("Light setting");
    ImGui::SliderFloat3("ambient", amb, 0.0, 1.0);
    lightAmbient[0] = amb[0];
    lightAmbient[1] = amb[1];
    lightAmbient[2] = amb[2];
    ImGui::SliderFloat3("diffuse", dif, 0.0, 1.0);
    lightDiffuse[0] = dif[0];
    lightDiffuse[1] = dif[1];
    lightDiffuse[2] = dif[2];
    ImGui::SliderFloat3("specular", spe, 0.0, 1.0);
    lightSpecular[0] = spe[0];
    lightSpecular[1] = spe[1];
    lightSpecular[2] = spe[2];
    ImGui::SliderFloat3("Position", pos, -10.0, 10.0);
    lightPos[0] = pos[0];
    lightPos[1] = pos[1];
    lightPos[2] = pos[2];
}

void setImObj(Cube* cubic) {
    ImGui::Text("object setting");
    ImGui::SliderFloat("shininess", &shininess, 0.0, 100);
    cubic->shininess = shininess;
}

int main() {

    // Initializing GLFW
    const char* glsl_version = "#version 300 es";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, "LearnOpenGL", NULL, NULL);
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

    // Depth testing, no weird artifact
    glEnable(GL_DEPTH_TEST);

    // Initializing imGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Create shader object
    Shader lightShader("vertex.vs", "fragment.fs");
    // Create cube object
    Cube testCube;

    // Rendering loop
    while (!glfwWindowShouldClose(window)) {

        // Push and pop
        MatrixStack MS;
       
        // Time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        // Input
        processInput(window);

        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // World placement and Camera
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

        setColor(lightShader, testCube);
        lightShader.setMV(model, view);
        
        testCube.drawCube("texture/container2.png","texture/container2_specular.png");
        
        model = MS.pop();

        // ------------------------------------------------------------------------------
        // Lighting Cube

        MS.push(model);

        //model = rotate(model, lastFrame * 1.0f, 0, 1, 0);
        model = translate(model, lightPos[0],lightPos[1], lightPos[2]);
        model = scale(model, 0.2f, 0.2f, 0.2f);

        setLight(lightShader,lightPos, lightAmbient, lightDiffuse, lightSpecular );
        lightShader.setMV(model, view);
        // Lighting position
        if (trackWithCam) {
            lightShader.setLightPos(testCam.cameraPos);
            lightShader.setLightDirect(testCam.cameraFront);
            lightShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
            lightShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
            lightShader.setIsSpot(1);
        }
        else {
            lightShader.setIsSpot(0);
            lightShader.setLightPos(glm::vec3(model[3]));
        }
        testCube.drawCube("","",true);
        
        model = MS.pop();

        // ImGui After scene
        ImGui::Begin("Parameters");
        ImGui::Text("press [3] : unbind from camera - press [4] : bind to camera");
        ImGui::Text("press [1] : light track camera - press [2] : light untrack camera");
        setImLight();
        setImObj(&testCube);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // update window
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    testCube.deleteBuffVer();
    glfwTerminate();
    return 0;
}