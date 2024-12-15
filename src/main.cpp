

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <openglDebug.h>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>

int glWindowWidth = 1920;
int glWindowHeight = 1080;
int retina_width, retina_height;
GLFWwindow *glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;
glm::mat4 lightRotation;

glm::vec3 lightDir;
glm::vec3 lightColor;

gps::Camera myCamera(
    glm::vec3(0.0f, 2.0f, 5.5f),
    glm::vec3(0.0f, 1.0f, 0.0f));
float baseCameraSpeed = 1.0f;
float cameraSpeed = baseCameraSpeed;

bool pressedKeys[1024];
float angleY = 0.0f;
bool firstMouse = true;
float lastX, lastY;

float timeWater = 0.0f;

int vertexWaveCount = 8;
int fragmentWaveCount = 40;

float vertexSeed = 0.0f;
float vertexSeedIter = 1253.2131f;
float vertexFrequency = 1.0f;
float vertexFrequencyMult = 1.18f;
float vertexAmplitude = 1.0f;
float vertexAmplitudeMult = 0.82f;
float vertexInitialSpeed = 2.0f;
float vertexSpeedRamp = 1.07f;
float vertexDrag = 1.0f;
float vertexHeight = 1.0f;
float vertexMaxPeak = 1.0f;
float vertexPeakOffset = 1.0f;

float fragmentSeed = 0.0f;
float fragmentSeedIter = 1253.2131f;
float fragmentFrequency = 1.0f;
float fragmentFrequencyMult = 1.18f;
float fragmentAmplitude = 1.0f;
float fragmentAmplitudeMult = 0.82f;
float fragmentInitialSpeed = 2.0f;
float fragmentSpeedRamp = 1.07f;
float fragmentDrag = 1.0f;
float fragmentHeight = 1.0f;
float fragmentMaxPeak = 1.0f;
float fragmentPeakOffset = 1.0f;

gps::Model3D water;
gps::Shader waterShader;

void windowResizeCallback(GLFWwindow *window, int width, int height)
{
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            pressedKeys[key] = true;
        else if (action == GLFW_RELEASE)
            pressedKeys[key] = false;
    }
}

void mouseCallback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        myCamera.ProcessMouseMovement(xoffset, yoffset);
    }

    lastX = xpos;
    lastY = ypos;
}

void processMovement()
{
    if (pressedKeys[GLFW_KEY_LEFT_SHIFT] || pressedKeys[GLFW_KEY_RIGHT_SHIFT])
    {
        cameraSpeed = baseCameraSpeed * 5.0f;
    }
    else
    {
        cameraSpeed = baseCameraSpeed;
    }
    if (pressedKeys[GLFW_KEY_W])
    {
        myCamera.ProcessKeyboard(gps::FORWARD, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_S])
    {
        myCamera.ProcessKeyboard(gps::BACKWARD, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_A])
    {
        myCamera.ProcessKeyboard(gps::LEFT, cameraSpeed);
    }

    if (pressedKeys[GLFW_KEY_D])
    {
        myCamera.ProcessKeyboard(gps::RIGHT, cameraSpeed);
    }
}

bool initOpenGLWindow()
{
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
    if (!glWindow)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
    glfwSetKeyCallback(glWindow, keyboardCallback);
    glfwSetCursorPosCallback(glWindow, mouseCallback);
    // glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(glWindow);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(1);

    /*glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);*/

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

    return true;
}

void initOpenGLState()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, retina_width, retina_height);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);  // cull face
    glCullFace(GL_BACK);     // cull back face
    glFrontFace(GL_CCW);     // GL_CCW for counter clock-wise

    glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects()
{
    water.LoadModel(RESOURCES_PATH "objects/water/water.obj");
}

void initShaders()
{
    waterShader.loadShader(RESOURCES_PATH "shaders/water.vert", RESOURCES_PATH "shaders/water.frag");
    waterShader.useShaderProgram();
}

void initUniforms()
{

    model = glm::mat4(1.0f);

    view = myCamera.getViewMatrix();

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

    projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 10000.0f);

    lightDir = glm::vec3(-400.0f, 500.0f, 1000.0f);

    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    waterShader.useShaderProgram();
    waterShader.setMat4("model", model);
    waterShader.setMat4("view", view);
    waterShader.setMat3("normalMatrix", normalMatrix);

    waterShader.setVec3("lightPos", lightDir);
    waterShader.setVec3("viewPos", myCamera.getCameraPosition());
    waterShader.setVec3("lightColor", lightColor);
    waterShader.setVec3("objectColor", glm::vec3(0.0f, 0.5f, 1.0f));


    waterShader.setInt("vertexWaveCount", vertexWaveCount);
    waterShader.setFloat("vertexSeed", vertexSeed);
    waterShader.setFloat("vertexSeedIter", vertexSeedIter);
    waterShader.setFloat("vertexFrequency", vertexFrequency);
    waterShader.setFloat("vertexFrequencyMult", vertexFrequencyMult);
    waterShader.setFloat("vertexAmplitude", vertexAmplitude);
    waterShader.setFloat("vertexAmplitudeMult", vertexAmplitudeMult);
    waterShader.setFloat("vertexInitialSpeed", vertexInitialSpeed);
    waterShader.setFloat("vertexSpeedRamp", vertexSpeedRamp);
    waterShader.setFloat("vertexDrag", vertexDrag);
    waterShader.setFloat("vertexHeight", vertexHeight);
    waterShader.setFloat("vertexMaxPeak", vertexMaxPeak);
    waterShader.setFloat("vertexPeakOffset", vertexPeakOffset);
    waterShader.setInt("fragmentWaveCount", fragmentWaveCount);
    waterShader.setFloat("fragmentSeed", fragmentSeed);
    waterShader.setFloat("fragmentSeedIter", fragmentSeedIter);
    waterShader.setFloat("fragmentFrequency", fragmentFrequency);
    waterShader.setFloat("fragmentFrequencyMult", fragmentFrequencyMult);
    waterShader.setFloat("fragmentAmplitude", fragmentAmplitude);
    waterShader.setFloat("fragmentAmplitudeMult", fragmentAmplitudeMult);
    waterShader.setFloat("fragmentInitialSpeed", fragmentInitialSpeed);
    waterShader.setFloat("fragmentSpeedRamp", fragmentSpeedRamp);
    waterShader.setFloat("fragmentDrag", fragmentDrag);
    waterShader.setFloat("fragmentHeight", fragmentHeight);
    waterShader.setFloat("fragmentMaxPeak", fragmentMaxPeak);
    waterShader.setFloat("fragmentPeakOffset", fragmentPeakOffset);
}

void drawWater(gps::Shader shader)
{
    shader.useShaderProgram();

    model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    shader.setMat3("normalMatrix", normalMatrix);

    water.Draw(shader);
}

void renderScene()
{

    glViewport(0, 0, retina_width, retina_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = myCamera.getViewMatrix();

    waterShader.useShaderProgram();
    waterShader.setMat4("view", view);
    waterShader.setVec3("viewPos", myCamera.getCameraPosition());
    waterShader.setVec3("lightPos", lightDir);
    waterShader.setMat4("projection", projection);
    waterShader.setFloat("time", timeWater);

    waterShader.setInt("vertexWaveCount", vertexWaveCount);
    waterShader.setFloat("vertexSeed", vertexSeed);
    waterShader.setFloat("vertexSeedIter", vertexSeedIter);
    waterShader.setFloat("vertexFrequency", vertexFrequency);
    waterShader.setFloat("vertexFrequencyMult", vertexFrequencyMult);
    waterShader.setFloat("vertexAmplitude", vertexAmplitude);
    waterShader.setFloat("vertexAmplitudeMult", vertexAmplitudeMult);
    waterShader.setFloat("vertexInitialSpeed", vertexInitialSpeed);
    waterShader.setFloat("vertexSpeedRamp", vertexSpeedRamp);
    waterShader.setFloat("vertexDrag", vertexDrag);
    waterShader.setFloat("vertexHeight", vertexHeight);
    waterShader.setFloat("vertexMaxPeak", vertexMaxPeak);
    waterShader.setFloat("vertexPeakOffset", vertexPeakOffset);
    waterShader.setInt("fragmentWaveCount", fragmentWaveCount);
    waterShader.setFloat("fragmentSeed", fragmentSeed);
    waterShader.setFloat("fragmentSeedIter", fragmentSeedIter);
    waterShader.setFloat("fragmentFrequency", fragmentFrequency);
    waterShader.setFloat("fragmentFrequencyMult", fragmentFrequencyMult);
    waterShader.setFloat("fragmentAmplitude", fragmentAmplitude);
    waterShader.setFloat("fragmentAmplitudeMult", fragmentAmplitudeMult);
    waterShader.setFloat("fragmentInitialSpeed", fragmentInitialSpeed);
    waterShader.setFloat("fragmentSpeedRamp", fragmentSpeedRamp);
    waterShader.setFloat("fragmentDrag", fragmentDrag);
    waterShader.setFloat("fragmentHeight", fragmentHeight);
    waterShader.setFloat("fragmentMaxPeak", fragmentMaxPeak);
    waterShader.setFloat("fragmentPeakOffset", fragmentPeakOffset);
    drawWater(waterShader);
}

void initImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(glWindow, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwDestroyWindow(glWindow);
    glfwTerminate();
}

int main(int argc, const char *argv[])
{

    if (!initOpenGLWindow())
    {
        glfwTerminate();
        return 1;
    }

    initOpenGLState();
    initObjects();
    initShaders();
    initUniforms();
    initImGui();

    while (!glfwWindowShouldClose(glWindow))
    {
        timeWater = (float)glfwGetTime();
        waterShader.useShaderProgram();
        processMovement();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Water Parameters");
        ImGui::SliderInt("Vertex Wave Count", &vertexWaveCount, 1, 20);
        ImGui::SliderFloat("Vertex Seed", &vertexSeed, 0.0f, 1000.0f);
        ImGui::SliderFloat("Vertex Seed Iter", &vertexSeedIter, 0.0f, 1000.0f);
        ImGui::SliderFloat("Vertex Frequency", &vertexFrequency, 0.1f, 10.0f);
        ImGui::SliderFloat("Vertex Frequency Mult", &vertexFrequencyMult, 0.1f, 10.0f);
        ImGui::SliderFloat("Vertex Amplitude", &vertexAmplitude, 0.0f, 1.0f);
        ImGui::SliderFloat("Vertex Amplitude Mult", &vertexAmplitudeMult, 0.0f, 1.0f);
        ImGui::SliderFloat("Vertex Initial Speed", &vertexInitialSpeed, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Speed Ramp", &vertexSpeedRamp, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Drag", &vertexDrag, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Height", &vertexHeight, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Max Peak", &vertexMaxPeak, 0.0f, 10.0f);
        ImGui::SliderFloat("Vertex Peak Offset", &vertexPeakOffset, 0.0f, 10.0f);
        ImGui::SliderInt("Fragment Wave Count", &fragmentWaveCount, 1, 50);
        ImGui::SliderFloat("Fragment Seed", &fragmentSeed, 0.0f, 1000.0f);
        ImGui::SliderFloat("Fragment Seed Iter", &fragmentSeedIter, 0.0f, 1000.0f);
        ImGui::SliderFloat("Fragment Frequency", &fragmentFrequency, 0.1f, 10.0f);
        ImGui::SliderFloat("Fragment Frequency Mult", &fragmentFrequencyMult, 0.1f, 10.0f);
        ImGui::SliderFloat("Fragment Amplitude", &fragmentAmplitude, 0.0f, 1.0f);
        ImGui::SliderFloat("Fragment Amplitude Mult", &fragmentAmplitudeMult, 0.0f, 1.0f);
        ImGui::SliderFloat("Fragment Initial Speed", &fragmentInitialSpeed, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Speed Ramp", &fragmentSpeedRamp, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Drag", &fragmentDrag, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Height", &fragmentHeight, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Max Peak", &fragmentMaxPeak, 0.0f, 10.0f);
        ImGui::SliderFloat("Fragment Peak Offset", &fragmentPeakOffset, 0.0f, 10.0f);
        ImGui::End();

        ImGui::Begin("Light Position");
        ImGui::SliderFloat("X", &lightDir.x, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Y", &lightDir.y, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Z", &lightDir.z, -1000.0f, 1000.0f);
        ImGui::End();

        renderScene();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;
}
