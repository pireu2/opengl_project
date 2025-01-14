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

#include <Shader.hpp>
#include <Model3D.hpp>
#include <Camera.hpp>
#include <SkyBox.hpp>
#include <Water.hpp>
#include <Atmosphere.hpp>
#include <Grass.hpp>
#include <Tree.hpp>
#include <Ground.hpp>
#include <CameraTour.hpp>

int glWindowWidth = 1920;
int glWindowHeight = 1080;
int retina_width, retina_height;
GLFWwindow *glWindow = nullptr;

constexpr unsigned int SHADOW_WIDTH = 8192;
constexpr unsigned int SHADOW_HEIGHT = 8192;

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

glm::vec3 lightDir;
glm::vec3 lightColor;
glm::mat4 lightRotation;
float lightAngle;

gps::Camera myCamera(
    glm::vec3(0.0f, 2.0f, 5.5f),
    glm::vec3(0.0f, 1.0f, 0.0f));
float baseCameraSpeed = 1.0f;
float cameraSpeed = baseCameraSpeed;
gps::CameraTour cameraTour;

bool pressedKeys[1024];
bool firstMouse = true;
float lastX, lastY;

gps::Water water;
gps::Atmosphere atmosphere;
gps::Grass grass;
gps::Tree tree;
gps::Ground ground;

gps::SkyBox mySkybox;
gps::Shader skyboxShader;

gps::Model3D screenQuad;
gps::Shader screenQuadShader;

gps::Model3D lightHouse;
gps::Shader lightHouseShader;

auto pointLightPosition = glm::vec3(0.0f, 100.0f, 0.0f);
auto pointLightColor = glm::vec3(1.0f, 1.0f, 0.0f);

gps::Shader depthMapShader;

unsigned int framebuffer;
unsigned int textureColorBuffer;

unsigned int depthTexture;

unsigned int quadVAO = 0;
unsigned int quadVBO;

unsigned int shadowMapFBO;
unsigned int depthMapTexture;
bool showDepthMap = false;

bool wireframeMode = false;
bool pointViewMode = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void windowResizeCallback(GLFWwindow *window, int width, int height)
{
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (cameraTour.isTourActive())
        {
            cameraTour.stopTour();
        }
        else
        {
            cameraTour.loadCameraPositionsFromFile(RESOURCES_PATH "camera_positions.txt");
            cameraTour.startTour();
        }
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        showDepthMap = !showDepthMap;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        pointViewMode = !pointViewMode;
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        wireframeMode = !wireframeMode;
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        cameraTour.sampleCameraPosition(myCamera.getCameraPosition(), myCamera.getCameraFront());
    }

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
    if (cameraTour.isTourActive())
    {
        cameraTour.updateTour(deltaTime, myCamera, 2.0f);
    }
    else
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initObjects()
{
    water.loadModel(RESOURCES_PATH "objects/water/water.obj");
    grass.loadModel(RESOURCES_PATH "objects/grass/grass.obj");
    tree.loadModel(RESOURCES_PATH "objects/palm/palm.obj");
    ground.loadModel(RESOURCES_PATH "objects/ground/ground.obj");
    screenQuad.LoadModel(RESOURCES_PATH "objects/quad/quad.obj");
    lightHouse.LoadModel(RESOURCES_PATH "objects/lighthouse/lighthouse.obj");

    mySkybox.LoadFromDir(RESOURCES_PATH "skybox/");
    grass.init();
    tree.init();
    ground.init();
}

void initShaders()
{
    grass.loadShader(RESOURCES_PATH "shaders/grass.vert", RESOURCES_PATH "shaders/grass.frag");
    water.loadShader(RESOURCES_PATH "shaders/water.vert", RESOURCES_PATH "shaders/water.frag");
    atmosphere.loadShader(RESOURCES_PATH "shaders/atmosphere.vert", RESOURCES_PATH "shaders/atmosphere.frag");
    ground.loadShader(RESOURCES_PATH "shaders/ground.vert", RESOURCES_PATH "shaders/ground.frag");
    tree.loadShader(RESOURCES_PATH "shaders/tree.vert", RESOURCES_PATH "shaders/tree.frag");

    skyboxShader.loadShader(RESOURCES_PATH "shaders/skyboxShader.vert", RESOURCES_PATH "shaders/skyboxShader.frag");
    skyboxShader.useShaderProgram();

    screenQuadShader.loadShader(RESOURCES_PATH "shaders/screenQuad.vert", RESOURCES_PATH "shaders/screenQuad.frag");
    screenQuadShader.useShaderProgram();

    lightHouseShader.loadShader(RESOURCES_PATH "shaders/lightHouse.vert", RESOURCES_PATH "shaders/lightHouse.frag");
    lightHouseShader.useShaderProgram();

    depthMapShader.loadShader(RESOURCES_PATH "shaders/depthShaders/depthMap.vert", RESOURCES_PATH "shaders/depthShaders/depthMap.frag");
    depthMapShader.useShaderProgram();
}

void initUniforms()
{
    model = glm::mat4(1.0f);

    view = myCamera.getViewMatrix();

    normalMatrix = glm::mat3(inverseTranspose(view * model));

    projection = glm::perspective(glm::radians(myCamera.getZoom()), static_cast<float>(retina_width) / static_cast<float>(retina_height), 0.1f, 10000.0f);

    lightDir = glm::vec3(6.622f, 300.0f, -515.225f);

    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    lightRotation = rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::vec3 lightDirTr = inverseTranspose(glm::mat3(view * lightRotation)) * lightDir;

    skyboxShader.useShaderProgram();
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);

    const auto cameraPosition = myCamera.getCameraPosition();

    lightHouseShader.useShaderProgram();
    model = scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    model = translate(model, glm::vec3(0.0f, 11.0f, 0.0f));
    lightHouseShader.setMat4("model", model);
    lightHouseShader.setMat4("view", view);
    lightHouseShader.setMat4("projection", projection);
    lightHouseShader.setMat3("normalMatrix", normalMatrix);
    lightHouseShader.setVec3("lightDir", value_ptr(lightDirTr));
    lightHouseShader.setVec3("lightColor", value_ptr(lightColor));

    model = glm::mat4(1.0f);

    water.initUniforms(model, view, normalMatrix, lightDirTr, lightColor, cameraPosition);

    ground.initUniforms(model, view, projection, normalMatrix, lightDirTr, lightColor);

    grass.initUniforms(model, view, projection, normalMatrix, lightDirTr, lightColor);

    model = scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    tree.initUniforms(model, view, projection, normalMatrix, lightDirTr, lightColor);

    model = glm::mat4(1.0f);
}

void initFrameBuffer()
{
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

    grass.setInstancePositions(grass.generateGrassPositions());
    tree.setInstancePositions(tree.generateTreePositions());

    glGenFramebuffers(1, &shadowMapFBO);
    glGenTextures(1, &depthMapTexture);

    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    constexpr float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(stderr, "ERROR: Framebuffer is not complete\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix()
{
    const glm::mat4 lightView = lookAt(inverseTranspose(glm::mat3(lightRotation)) * lightDir,
                                       glm::vec3(0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, 0.1f, 4500.0f);
    const glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

    return lightSpaceTrMatrix;
}

void renderQuad()
{
    if (quadVAO == 0)
    {
        constexpr float quadVertices[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void renderScene()
{
    const auto lightSpaceTrMatrix = computeLightSpaceTrMatrix();
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ground.render_depth(lightSpaceTrMatrix);
    tree.render_depth(lightSpaceTrMatrix);

    depthMapShader.useShaderProgram();
    depthMapShader.setMat4("lightSpaceTrMatrix", value_ptr(lightSpaceTrMatrix));
    model = scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    model = translate(model, glm::vec3(0.0f, 11.0f, 0.0f));
    depthMapShader.setMat4("model", value_ptr(model));
    model = glm::mat4(1.0f);
    lightHouse.Draw(depthMapShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (showDepthMap)
    {
        glViewport(0, 0, retina_width, retina_height);
        glClear(GL_COLOR_BUFFER_BIT);
        screenQuadShader.useShaderProgram();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        screenQuadShader.setInt("depthMap", 0);

        glDisable(GL_DEPTH_TEST);
        screenQuad.Draw(screenQuadShader);
        glEnable(GL_DEPTH_TEST);
    }
    else
    {

        if (wireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else if (pointViewMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glViewport(0, 0, retina_width, retina_height);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = myCamera.getViewMatrix();
        projection = glm::perspective(glm::radians(myCamera.getZoom()), static_cast<float>(retina_width) / static_cast<float>(retina_height), 0.1f, 10000.0f);
        normalMatrix = glm::mat3(inverseTranspose(view * model));
        lightRotation = rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::vec3 lightDirTr = inverseTranspose(glm::mat3(view * lightRotation)) * lightDir;

        const auto cameraPosition = myCamera.getCameraPosition();

        lightHouseShader.useShaderProgram();
        lightHouseShader.setMat4("view", view);
        lightHouseShader.setMat4("projection", projection);
        lightHouseShader.setMat3("normalMatrix", normalMatrix);
        lightHouseShader.setVec3("lightDir", value_ptr(lightDirTr));
        lightHouseShader.setVec3("lightColor", value_ptr(lightColor));
        lightHouseShader.setVec3("pointLightPosition", value_ptr(pointLightPosition));
        lightHouseShader.setVec3("pointLightColor", value_ptr(pointLightColor));
        lightHouse.Draw(lightHouseShader);

        // Render ground
        ground.render(view, projection, normalMatrix, lightDirTr, lightColor, lightSpaceTrMatrix, depthMapTexture, pointLightPosition, pointLightColor);

        // Render Grass
        grass.render(view, projection, normalMatrix, lightDirTr, lightColor, lightSpaceTrMatrix, depthMapTexture, pointLightPosition, pointLightColor);

        // Render Trees
        tree.render(view, projection, normalMatrix, lightDirTr, lightColor, lightSpaceTrMatrix, depthMapTexture, pointLightPosition, pointLightColor);

        // Render Skybox
        skyboxShader.useShaderProgram();
        skyboxShader.setMat4("projection", projection);
        skyboxShader.setMat4("view", view);
        mySkybox.Draw(skyboxShader, view, projection);

        // Render Water
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mySkybox.GetTextureId());
        water.render(model, view, projection, normalMatrix, lightDirTr, lightColor, cameraPosition);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glClear(GL_COLOR_BUFFER_BIT);

        atmosphere.setUniforms(view, projection, cameraPosition, static_cast<float>(glfwGetTime()));

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        atmosphere.setSceneTexture(4);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        atmosphere.setDepthTexture(1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mySkybox.GetTextureId());
        atmosphere.setSkyboxTexture(2);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        renderQuad();

        glEnable(GL_DEPTH_TEST);
    }
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
    glDeleteTextures(1, &textureColorBuffer);
    glDeleteTextures(1, &depthTexture);
    glDeleteTextures(1, &depthMapTexture);
    glDeleteFramebuffers(1, &shadowMapFBO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteVertexArrays(1, &quadVAO);
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
    initFrameBuffer();

    while (!glfwWindowShouldClose(glWindow))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processMovement();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Light");
        ImGui::DragFloat3("Light Direction", value_ptr(lightDir), 1.0f, -1000.0f, 1000.0f);
        ImGui::DragFloat("Light Angle", &lightAngle, 1.0f, 0.0f, 360.0f);
        ImGui::ColorEdit3("Light Color", value_ptr(lightColor));

        // point light controls
        ImGui::DragFloat3("Point Light Position", value_ptr(pointLightPosition), 1.0f, -1000.0f, 1000.0f);
        ImGui::ColorEdit3("Point Light Color", value_ptr(pointLightColor));

        ImGui::End();

        renderScene();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    if (!cameraTour.isEmpty())
    {
        cameraTour.saveCameraPositionsToFile(RESOURCES_PATH "camera_positions.txt");
    }

    cleanup();

    return 0;
}
