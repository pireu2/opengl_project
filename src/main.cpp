

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
}

void initUniforms()
{

    model = glm::mat4(1.0f);

    view = myCamera.getViewMatrix();

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

    projection = glm::perspective(glm::radians(myCamera.getZoom()), static_cast<float>(retina_width) / static_cast<float>(retina_height), 0.1f, 10000.0f);

    lightPos = glm::vec3(-59.0f, 180.0f, -550.0f);

    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    skyboxShader.useShaderProgram();
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);

    const auto cameraPosition = myCamera.getCameraPosition();

    water.initUniforms(model, view, normalMatrix, lightPos, lightColor, cameraPosition);
}

void renderScene()
{

    glViewport(0, 0, retina_width, retina_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = myCamera.getViewMatrix();
    projection = glm::perspective(glm::radians(myCamera.getZoom()), static_cast<float>(retina_width) / static_cast<float>(retina_height), 0.1f, 10000.0f);
    const auto cameraPosition = myCamera.getCameraPosition();

    skyboxShader.useShaderProgram();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setMat4("view", view);

    water.setUniforms(view, projection, lightPos, cameraPosition);

    water.draw(view);
    mySkybox.Draw(skyboxShader, view, projection);
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

    initOpenGLState();
    initObjects();
    initShaders();
    initUniforms();
    initImGui();

    mySkybox.LoadFromDir(RESOURCES_PATH "skybox/variant1/");

    while (!glfwWindowShouldClose(glWindow))
    {
        const auto timeWater = static_cast<float>(glfwGetTime());
        water.setTime(timeWater);

        processMovement();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        water.drawImguiControls();


        ImGui::Begin("Light Position");
        ImGui::SliderFloat("X", &lightPos.x, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Y", &lightPos.y, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Z", &lightPos.z, -1000.0f, 1000.0f);
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
