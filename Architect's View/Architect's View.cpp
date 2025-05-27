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
#include <vector>
#include <stdlib.h>  

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
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -3.0f);
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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cameraPos.z = 3;
        cameraPos.x = 0.0;
        cameraPos.y = 4.0;
    }  
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
    return glm::rotate(model, glm::radians(degree), glm::vec3(x,y,z));
}

void setColor(Shader shader, Cube cubic) {
    shader.setColor(cubic.shininess);
    shader.setID("ID", 0);
}

void setLight(Shader shader, glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
    shader.setLight(pos, ambient, diffuse, specular);
    shader.setFloat("light.constant", 1.0f);
    shader.setFloat("light.linear", 0.0f);
    shader.setFloat("light.quadratic",0.0f);
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

std::vector<Cube> setWalls(const char* path, int* numCubes) {

    std::ifstream file(path);
    std::string strAmount;
    std::getline(file, strAmount);
    int amount = std::stoi(strAmount);
    *numCubes = amount;
    std::vector<Cube> tempVecCubes;
    std::string fileLine;
    std::vector<std::string> tokens;
    while (std::getline(file, fileLine)) {
        std::stringstream noLineFile(fileLine);
        std::string token;
        while (std::getline(noLineFile, token, '-')) {
            tokens.push_back(token);
        }
    }
    for (int i = 0; i < amount; i++) {
        Cube tempCube;
        int ID = std::stoi(tokens[5*i+0]);
        float sX = std::stof(tokens[5*i+1]);
        float sY = std::stof(tokens[5*i+2]);
        float sZ = std::stof(tokens[5*i+3]);
        float rotate = std::stof(tokens[5*i+4]);
        tempCube.setTransformation(ID, glm::vec3(0.0, 0.0, 0.0), glm::vec3(sX, sY, sZ), rotate);
        tempVecCubes.push_back(tempCube);
    }
       
        
    file.close();

    return tempVecCubes;
}

void swap(Cube& cube1, Cube& cube2) {
    Cube temp = cube1;
    cube1 = cube2;
    cube2 = temp;
}

int partition(std::vector<Cube>& arr, int low, int high) {
    int pivot = arr[high].id; 
    int i = (low - 1);  

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].id < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void sortCube(std::vector<Cube>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        sortCube(arr, low, pi - 1);
        sortCube(arr, pi + 1, high);
    }
}



int main() {

    int numCubes = 0;
    std::vector<Cube> arrayCubeSpec;
    arrayCubeSpec = setWalls("detection.txt", &numCubes);
    sortCube(arrayCubeSpec, 0, numCubes - 1);

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
     
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    arrayCubeSpec[0].setTexture("texture/container2.png", "texture/container2_specular.png");
    arrayCubeSpec[1].setTexture("texture/wall.jpg", "texture/container2_specular.png");
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
        lightShader.setProj(projection);
        lightShader.setView(view);
        

        // Scene
       
        // Plane
        MS.push(model);
        model = translate(model, 0.0f, -1.0f, 0.0f);
        model = scale(model,10.f, 1.0f, 10.0f);
        setColor(lightShader, arrayCubeSpec[0]);
        lightShader.setModel(model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        model = MS.pop();
     
        //drawAlgorithm(numCubes, &MS, model, arrayCubeSpec, &lightShader, &testCube);
        
        
        float prevX = 0.0;
        float prevY = 0.0;
        float prevZ = 0.0;
        float prevRotate = 0.0;
        MS.push(model);
        for (int i = 0; i < 4; i++) {
            
            if (i != 0) {
                model = translate(model, prevX, prevY, prevZ);
                model = rotate(model, -prevRotate, 0, 1, 0);
                model = rotate(model, arrayCubeSpec[i].rotate, 0, 1, 0);
                model = translate(model, arrayCubeSpec[i].scale.x / 2, prevY, prevZ);
            }
            // Draw cube
            MS.push(model);
                model = scale(model, arrayCubeSpec[i].scale.x, arrayCubeSpec[i].scale.y, arrayCubeSpec[i].scale.z);

                setColor(lightShader, arrayCubeSpec[i]);
                lightShader.setModel(model);
                
                // Draw
                // bind diffuse map
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, arrayCubeSpec[i].diffuseMap);
                // bind specular map
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, arrayCubeSpec[i].specularMap);
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glActiveTexture(GL_TEXTURE0);
                
            model = MS.pop();

            prevX = arrayCubeSpec[i].scale.x / 2;
            prevRotate = arrayCubeSpec[i].rotate;
        }
        model = MS.pop();
        
        // ------------------------------------------------------------------------------
        // Lighting Cube

        MS.push(model);

        model = translate(model, lightPos[0],lightPos[1], lightPos[2]);
        model = scale(model, 0.2f, 0.2f, 0.2f);

        setLight(lightShader,lightPos, lightAmbient, lightDiffuse, lightSpecular );
        lightShader.setModel(model);
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
        // Draw
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
        model = MS.pop();

        // ImGui After scene
        ImGui::Begin("Parameters");
        ImGui::Text("press [3] : unbind from camera - press [4] : bind to camera");
        ImGui::Text("press [1] : light track camera - press [2] : light untrack camera");
        setImLight();
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

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
    
}