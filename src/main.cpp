

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>ßßßßßßß
#include <backends/imgui_impl_opengl3.h>

#include <openglDebug.h>

#include <Shader.hpp>
#include <Model3D.hpp>
#include <Camera.hpp>
#include <SkyBox.hpp>
#include <Water.hpp>

int glWindowWidth = 1920;
int glWindowHeight = 1080;
int retina_width, retina_height;
GLFWwindow *glWindow = nullptr;

constexpr unsigned int SHADOW_WIDTH = 2048;
constexpr unsigned int SHADOW_HEIGHT = 2048;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;
glm::mat4 lightRotation;

glm::vec3 lightPos;
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


gps::Water water;

gps::SkyBox mySkybox;
gps::Shader skyboxShader;

gps::Shader fogShader;
float fogDensity = 0.5f;
float fogOffset = 3555.0f;
float fogHeight = 500.0f;

float fogAttenuation = 1.0f;
glm::vec3 fogColor = glm::vec3(1.0f, 0.95f, 0.9f);
float distanceFog = 3560.0f;

glm::vec3 sunColor = glm::vec3(1.0f, 0.7f, 0.0f);
glm::vec3 sunDirection = glm::vec3(-0.024f, -0.062f, 1.0f);


float skyboxSpeed = 0.05f;
glm::vec3 skyboxDirection = glm::vec3(0.0f, 1.0f, 0.0f);


unsigned int framebuffer;
unsigned int textureColorBuffer;
unsigned int rbo;

unsigned int depthTexture;

unsigned int quadVAO = 0;
unsigned int quadVBO;

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

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    myCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouseCallback(GLFWwindow *window, double xposIn, double yposIn)
{
    const auto xpos = static_cast<float>(xposIn);
    const auto ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        const float xoffset = xpos - lastX;
        const float yoffset = lastY - ypos;

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

    glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", nullptr, nullptr);
    if (!glWindow)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
    glfwSetKeyCallback(glWindow, keyboardCallback);
    glfwSetCursorPosCallback(glWindow, mouseCallback);
    glfwSetScrollCallback(glWindow, scrollCallback);
    // glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(glWindow);

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    glfwSwapInterval(1);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", reinterpret_cast<const char *>(renderer));
    printf("OpenGL version supported %s\n", reinterpret_cast<const char *>(version));

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
    water.loadModel(RESOURCES_PATH "objects/water/water.obj");
}

void initShaders()
{
    water.loadShader(RESOURCES_PATH "shaders/water.vert", RESOURCES_PATH "shaders/water.frag");
    skyboxShader.loadShader(RESOURCES_PATH "shaders/skyboxShader.vert", RESOURCES_PATH "shaders/skyboxShader.frag");
    skyboxShader.useShaderProgram();
    fogShader.loadShader(RESOURCES_PATH "shaders/fog.vert", RESOURCES_PATH "shaders/fog.frag");
    fogShader.useShaderProgram();
}

void initUniforms()
{

    model = glm::mat4(1.0f);

    view = myCamera.getViewMatrix();

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

    projection = glm::perspective(glm::radians(myCamera.getZoom()), static_cast<float>(retina_width) / static_cast<float>(retina_height), 0.1f, 10000.0f);

    lightPos = glm::vec3(6.622f, 218.543f, -515.225f);

    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    skyboxShader.useShaderProgram();
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);

    const auto cameraPosition = myCamera.getCameraPosition();

    water.initUniforms(model, view, normalMatrix, lightPos, lightColor, cameraPosition);
}

void initFrameBuffer() {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, retina_width, retina_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, retina_width, retina_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "ERROR: Framebuffer is not complete\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderQuad() {
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}


void renderScene() {
    glViewport(0, 0, retina_width, retina_height);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = myCamera.getViewMatrix();
    projection = glm::perspective(glm::radians(myCamera.getZoom()), static_cast<float>(retina_width) / static_cast<float>(retina_height), 0.1f, 10000.0f);
    const auto cameraPosition = myCamera.getCameraPosition();

    // Render Skybox
    skyboxShader.useShaderProgram();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setMat4("view", view);
    mySkybox.Draw(skyboxShader, view, projection);

    // Render Water
    water.setUniforms(view, projection, lightPos, cameraPosition);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mySkybox.GetTextureId());
    water.draw(view);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT);

    // Update and use fog shader
    fogShader.useShaderProgram();
    fogShader.setFloat("fogDensity", fogDensity);
    fogShader.setVec3("fogColor", fogColor);
    fogShader.setFloat("fogOffset", fogOffset);
    fogShader.setVec3("sunColor", sunColor);
    fogShader.setVec3("sunDirection", sunDirection);
    fogShader.setFloat("fogHeight", fogHeight);
    fogShader.setFloat("fogAttenuation", fogAttenuation);
    fogShader.setFloat("skyboxSpeed", skyboxSpeed);
    fogShader.setVec3("skyboxDirection", skyboxDirection);
    fogShader.setMat4("cameraInvViewProjection", glm::value_ptr(glm::inverse(projection * view)));
    fogShader.setVec3("cameraPos", glm::value_ptr(cameraPosition));
    fogShader.setFloat("distanceFog", distanceFog);
    fogShader.setFloat("time", static_cast<float>(glfwGetTime()));

    // Bind textures and set uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    fogShader.setInt("scene", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    fogShader.setInt("depthTexture", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mySkybox.GetTextureId());
    fogShader.setInt("skyboxTex", 2);

    renderQuad();
}

void initImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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

    mySkybox.LoadFromDir(RESOURCES_PATH "skybox/");

    initOpenGLState();
    initObjects();
    initShaders();
    initUniforms();
    initImGui();
    initFrameBuffer();


    while (!glfwWindowShouldClose(glWindow))
    {
        const auto timeWater = static_cast<float>(glfwGetTime());
        water.setTime(timeWater);

        processMovement();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        water.drawImguiControls();


        // ImGui::Begin("Light Position");
        // ImGui::SliderFloat("X", &lightPos.x, -1000.0f, 1000.0f);
        // ImGui::SliderFloat("Y", &lightPos.y, -1000.0f, 1000.0f);
        // ImGui::SliderFloat("Z", &lightPos.z, -1000.0f, 1000.0f);
        // ImGui::End();
        //
        //
        // ImGui::Begin("Fog Parameters");
        // ImGui::SliderFloat("Fog Density", &fogDensity, 0.0f, 10.0f);
        // ImGui::ColorEdit3("Fog Color", (float *)&fogColor);
        // ImGui::SliderFloat("Fog Offset", &fogOffset, 0.0f, 4000.0f);
        // ImGui::SliderFloat("Fog Height", &fogHeight, 0.0f, 1000.0f);
        // ImGui::SliderFloat("Fog Attenuation", &fogAttenuation, 0.0f, 1.0f);
        // ImGui::SliderFloat("Distance Fog", &distanceFog, 0.0f, 4000.0f);
        // ImGui::ColorEdit3("Sun Color", (float *)&sunColor);
        // ImGui::SliderFloat("Sun Direction X", &sunDirection.x, -1.0f, 1.0f);
        // ImGui::SliderFloat("Sun Direction Y", &sunDirection.y, -1.0f, 1.0f);
        // ImGui::SliderFloat("Sun Direction Z", &sunDirection.z, -1.0f, 1.0f);
        // ImGui::SliderFloat("Skybox Speed", &skyboxSpeed, 0.0f, 1.0f);
        // ImGui::SliderFloat("Skybox Direction X", &skyboxDirection.x, -1.0f, 1.0f);
        // ImGui::SliderFloat("Skybox Direction Y", &skyboxDirection.y, -1.0f, 1.0f);
        // ImGui::SliderFloat("Skybox Direction Z", &skyboxDirection.z, -1.0f, 1.0f);
        //
        // ImGui::End();

        renderScene();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;
}
