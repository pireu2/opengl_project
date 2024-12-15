

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/matrix_inverse.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include <openglDebug.h>

// #include "Shader.hpp"
// #include "Model3D.hpp"
// #include "Camera.hpp"
// #include "SkyBox.hpp"

// #include <iostream>

// int glWindowWidth = 800;
// int glWindowHeight = 600;
// int retina_width, retina_height;
// GLFWwindow* glWindow = NULL;

// const unsigned int SHADOW_WIDTH = 2048;
// const unsigned int SHADOW_HEIGHT = 2048;

// glm::mat4 model;
// glm::mat4 view;
// glm::mat4 projection;
// glm::mat3 normalMatrix;
// glm::mat4 lightRotation;

// glm::vec3 lightDir;
// glm::vec3 lightColor;

// gps::Camera myCamera(
//     glm::vec3(0.0f, 2.0f, 5.5f),
//     glm::vec3(0.0f, 1.0f, 0.0f));
// float cameraSpeed = 0.01f;

// bool pressedKeys[1024];
// float angleY = 0.0f;
// bool firstMouse = true;
// float lastX, lastY;

// float timeWater = 0.0f;

// GLfloat lightAngle;

// gps::Model3D nanosuit;
// gps::Model3D ground;
// gps::Model3D lightCube;
// gps::Model3D screenQuad;
// gps::Model3D city;

// gps::Model3D water;

// gps::Shader myCustomShader;
// gps::Shader lightShader;
// gps::Shader screenQuadShader;
// gps::Shader skyboxShader;
// gps::Shader depthMapShader;
// gps::Shader waterShader;

// gps::SkyBox mySkybox;

// GLuint shadowMapFBO;
// GLuint depthMapTexture;

// bool showDepthMap;

// void windowResizeCallback(GLFWwindow* window, int width, int height)
// {
//     fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
// }

// void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
// {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GL_TRUE);

//     if (key == GLFW_KEY_M && action == GLFW_PRESS)
//         showDepthMap = !showDepthMap;

//     if (key >= 0 && key < 1024)
//     {
//         if (action == GLFW_PRESS)
//             pressedKeys[key] = true;
//         else if (action == GLFW_RELEASE)
//             pressedKeys[key] = false;
//     }
// }

// void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
// {
//     float xpos = static_cast<float>(xposIn);
//     float ypos = static_cast<float>(yposIn);

//     if (firstMouse)
//     {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }

//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos;

//     lastX = xpos;
//     lastY = ypos;

//     myCamera.ProcessMouseMovement(xoffset, yoffset);
// }

// void processMovement()
// {
//     if (pressedKeys[GLFW_KEY_Q])
//     {
//         angleY -= 1.0f;
//     }

//     if (pressedKeys[GLFW_KEY_E])
//     {
//         angleY += 1.0f;
//     }

//     if (pressedKeys[GLFW_KEY_J])
//     {
//         lightAngle -= 1.0f;
//     }

//     if (pressedKeys[GLFW_KEY_L])
//     {
//         lightAngle += 1.0f;
//     }

//     if (pressedKeys[GLFW_KEY_W])
//     {
//         myCamera.ProcessKeyboard(gps::FORWARD, cameraSpeed);
//     }

//     if (pressedKeys[GLFW_KEY_S])
//     {
//         myCamera.ProcessKeyboard(gps::BACKWARD, cameraSpeed);
//     }

//     if (pressedKeys[GLFW_KEY_A])
//     {
//         myCamera.ProcessKeyboard(gps::LEFT, cameraSpeed);
//     }

//     if (pressedKeys[GLFW_KEY_D])
//     {
//         myCamera.ProcessKeyboard(gps::RIGHT, cameraSpeed);
//     }
// }

// bool initOpenGLWindow()
// {
//     if (!glfwInit())
//     {
//         fprintf(stderr, "ERROR: could not start GLFW3\n");
//         return false;
//     }

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

//     glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

//     glfwWindowHint(GLFW_SAMPLES, 4);

//     glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

//     glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
//     if (!glWindow)
//     {
//         fprintf(stderr, "ERROR: could not open window with GLFW3\n");
//         glfwTerminate();
//         return false;
//     }

//     glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
//     glfwSetKeyCallback(glWindow, keyboardCallback);
//     glfwSetCursorPosCallback(glWindow, mouseCallback);
//     glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

//     glfwMakeContextCurrent(glWindow);

//     gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

//     glfwSwapInterval(1);

//     /*glEnable(GL_DEBUG_OUTPUT);
//     glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//     glDebugMessageCallback(glDebugOutput, 0);
//     glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);*/

//     const GLubyte* renderer = glGetString(GL_RENDERER);
//     const GLubyte* version = glGetString(GL_VERSION);
//     printf("Renderer: %s\n", renderer);
//     printf("OpenGL version supported %s\n", version);

//     glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

//     return true;
// }

// void initOpenGLState()
// {
//     glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
//     glViewport(0, 0, retina_width, retina_height);

//     glEnable(GL_DEPTH_TEST); // enable depth-testing
//     glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"
//     glEnable(GL_CULL_FACE);  // cull face
//     glCullFace(GL_BACK);     // cull back face
//     glFrontFace(GL_CCW);     // GL_CCW for counter clock-wise

//     glEnable(GL_FRAMEBUFFER_SRGB);
// }

// void initObjects()
// {
//     nanosuit.LoadModel(RESOURCES_PATH "objects/nanosuit/nanosuit.obj");
//     ground.LoadModel(RESOURCES_PATH "objects/ground/ground.obj");
//     lightCube.LoadModel(RESOURCES_PATH "objects/cube/cube.obj");
//     screenQuad.LoadModel(RESOURCES_PATH "objects/quad/quad.obj");
//     water.LoadModel(RESOURCES_PATH "objects/water/water.obj");
//     // city.LoadModel(RESOURCES_PATH "objects/city/gm_bigcity.obj");
// }

// void initSkybox()
// {
//     std::vector<const GLchar*> faces;
//     faces.push_back(RESOURCES_PATH "skybox/right.tga");
//     faces.push_back(RESOURCES_PATH "skybox/left.tga");
//     faces.push_back(RESOURCES_PATH "skybox/top.tga");
//     faces.push_back(RESOURCES_PATH "skybox/bottom.tga");
//     faces.push_back(RESOURCES_PATH "skybox/back.tga");
//     faces.push_back(RESOURCES_PATH "skybox/front.tga");

//     mySkybox.Load(faces);
// }

// void initShaders()
// {
//     myCustomShader.loadShader(RESOURCES_PATH "shaders/shaderStart.vert", RESOURCES_PATH "shaders/shaderStart.frag");
//     myCustomShader.useShaderProgram();
//     lightShader.loadShader(RESOURCES_PATH "shaders/lightCube.vert", RESOURCES_PATH "shaders/lightCube.frag");
//     lightShader.useShaderProgram();
//     screenQuadShader.loadShader(RESOURCES_PATH "shaders/screenQuad.vert", RESOURCES_PATH "shaders/screenQuad.frag");
//     screenQuadShader.useShaderProgram();
//     skyboxShader.loadShader(RESOURCES_PATH "shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
//     skyboxShader.useShaderProgram();
//     depthMapShader.loadShader(RESOURCES_PATH "shaders/depthMapShader.vert", RESOURCES_PATH "shaders/depthMapShader.frag");
//     depthMapShader.useShaderProgram();
//     waterShader.loadShader(RESOURCES_PATH "shaders/water.vert", RESOURCES_PATH "shaders/water.frag");
//     waterShader.useShaderProgram();
// }

// void initUniforms()
// {
//     myCustomShader.useShaderProgram();

//     model = glm::mat4(1.0f);
//     myCustomShader.setMat4("model", model);

//     view = myCamera.getViewMatrix();
//     myCustomShader.setMat4("view", view);

//     normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
//     myCustomShader.setMat3("normalMatrix", normalMatrix);

//     projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
//     myCustomShader.setMat4("projection", projection);

//     lightDir = glm::vec3(1.0f, 1.0f, 0.0f);
//     lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
//     myCustomShader.setVec3("lightDir", glm::value_ptr(glm::inverseTranspose(glm::mat3(lightRotation)) * lightDir));

//     lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
//     myCustomShader.setVec3("lightColor", lightColor);

//     lightShader.useShaderProgram();
//     lightShader.setMat4("projection", projection);

//     waterShader.useShaderProgram();
//     waterShader.setMat4("model", model);
//     waterShader.setMat4("view", view);
//     waterShader.setMat3("normalMatrix", normalMatrix);

//     waterShader.setVec3("lightDir", lightDir);
//     waterShader.setVec3("lightColor", lightColor);
//     waterShader.setVec3("objectColor", glm::vec3(0.0f, 0.5f, 1.0f));
// }

// void initFBO()
// {
//     glGenFramebuffers(1, &shadowMapFBO);
//     glGenTextures(1, &depthMapTexture);

//     glBindTexture(GL_TEXTURE_2D, depthMapTexture);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
//         SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//     glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

//     glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
//     glDrawBuffer(GL_NONE);
//     glReadBuffer(GL_NONE);
// }

// glm::mat4 computeLightSpaceTrMatrix()
// {
//     glm::mat4 lightView = glm::lookAt(glm::inverseTranspose(glm::mat3(lightRotation)) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//     const GLfloat near_plane = 0.1f, far_plane = 6.0f;

//     glm::mat4 lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);

//     glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

//     return lightSpaceTrMatrix;
// }

// void drawObjects(gps::Shader shader, bool depthPass)
// {

//     shader.useShaderProgram();

//     model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
//     shader.setMat4("model", model);

//     if (!depthPass)
//     {
//         normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
//         shader.setMat3("normalMatrix", normalMatrix);
//     }

//     nanosuit.Draw(shader);

//     model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
//     model = glm::scale(model, glm::vec3(0.5f));
//     shader.setMat4("model", model);

//     if (!depthPass)
//     {
//         normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
//         shader.setMat3("normalMatrix", normalMatrix);
//     }

//     ground.Draw(shader);
// }

// void drawWater(gps::Shader shader)
// {
//     shader.useShaderProgram();

//     model = glm::mat4(1.0f);
//     shader.setMat4("model", model);

//     normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
//     shader.setMat3("normalMatrix", normalMatrix);

//     water.Draw(shader);
// }

// void renderScene()
// {

//     depthMapShader.useShaderProgram();
//     depthMapShader.setMat4("lightSpaceTrMatrix", computeLightSpaceTrMatrix());
//     glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
//     glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
//     glClear(GL_DEPTH_BUFFER_BIT);
//     drawObjects(depthMapShader, true);
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);

//     if (showDepthMap)
//     {
//         glViewport(0, 0, retina_width, retina_height);

//         glClear(GL_COLOR_BUFFER_BIT);

//         screenQuadShader.useShaderProgram();

//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, depthMapTexture);
//         screenQuadShader.setInt("depthMap", 0);

//         glDisable(GL_DEPTH_TEST);
//         screenQuad.Draw(screenQuadShader);
//         glEnable(GL_DEPTH_TEST);
//     }
//     else
//     {

//         glViewport(0, 0, retina_width, retina_height);

//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         myCustomShader.useShaderProgram();

//         view = myCamera.getViewMatrix();
//         myCustomShader.setMat4("view", view);

//         myCustomShader.useShaderProgram();

//         lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
//         myCustomShader.setVec3("lightDir", glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

//         glActiveTexture(GL_TEXTURE3);
//         glBindTexture(GL_TEXTURE_2D, depthMapTexture);
//         myCustomShader.setInt("shadowMap", 3);
//         myCustomShader.setMat4("lightSpaceTrMatrix", computeLightSpaceTrMatrix());

//         drawObjects(myCustomShader, false);

//         lightShader.useShaderProgram();
//         lightShader.setMat4("view", view);

//         model = lightRotation;
//         model = glm::translate(model, 1.0f * lightDir);
//         model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
//         lightShader.setMat4("model", model);

//         lightCube.Draw(lightShader);
//         mySkybox.Draw(skyboxShader, view, projection);

//         waterShader.useShaderProgram();
//         waterShader.setMat4("view", view);
//         waterShader.setMat4("projection", projection);
//         waterShader.setFloat("time", timeWater);
//         waterShader.setVec3("lightDir", glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
//         drawWater(waterShader);
//     }
// }

// void cleanup()
// {
//     glDeleteTextures(1, &depthMapTexture);
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
//     glDeleteFramebuffers(1, &shadowMapFBO);
//     glfwDestroyWindow(glWindow);
//     glfwTerminate();
// }

// int main(int argc, const char* argv[])
// {

//     if (!initOpenGLWindow())
//     {
//         glfwTerminate();
//         return 1;
//     }

//     initOpenGLState();
//     initObjects();
//     initSkybox();
//     initShaders();
//     initUniforms();
//     initFBO();

//     while (!glfwWindowShouldClose(glWindow))
//     {
//         timeWater = (float)glfwGetTime();
//         waterShader.useShaderProgram();
//         waterShader.setFloat("time", timeWater);
//         processMovement();
//         renderScene();

//         glfwPollEvents();
//         glfwSwapBuffers(glWindow);
//     }

//     cleanup();

//     return 0;
// }
