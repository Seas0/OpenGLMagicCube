/* Project Name: OpenGL Magic Cube
*  Author: Seas0 (Liu Sichen)
*  Create Date: 2020/11/15
*  Last Modify Date: 2020/12/23
*  Comment: This Project uses the Core-profile of OpenGL above 3.3 instead of Immediate mode,
*           So may not be able to run on some old System.
*/

/* // DONE List:
*  [x] Initialize a window
*  [x] Manage buffer
*  [x] Draw a triangle
*  [x] Draw a rectangle
*  [x] Draw a cube
*  [x] Camera control
*  [x] Pan the cube
*  [x] Rotate the cube
*  [x] Draw a 2x2 Magic Cube
*  [x] Pan & Rotate whole Magic Cube
*  [x] Rotate each section of the Magic Cube
*  [x] Draw & Control whole Magic Cube
*  [x] Pure color mode
*  [x] Apply texture on the cube
*/

/* // TODO List:
*  [.] Optimize camera
*  [ ] Auto camera
*  [ ] Display info text
*  [ ] Random Shuffle
*  [ ] Auto resume
*  [ ] <W> Compile as Windows (TM) Screen Saver Program
*  --- May be more features ?
*/

#define STB_IMAGE_IMPLEMENTATION

// GLAD: A simple library to initialize OpenGL function
#include <glad/glad.h>

// GLFW: A simple library to manage OpenGL context
#include <GLFW/glfw3.h>

// GLM:  A library for matrix calculate & etc.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// stb_image: A simple library for texture loading
#include <stb_image.h>

// some standard headers for basic I/O
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <ctime>

// custom headers
// --------------
// load & compile & link shader program
#include "source/shaderLoader.h"

// generate view & projection matrix to implement camera control
#include "source/cameraSystem.h"

typedef unsigned char byte;

// call back functions that handle event
// -------------------------------------

// reset window size vars & change viewport
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// use process function to check key status every frame
// avoid keyboard call back flaws
void processInput(GLFWwindow *window);

// mantain index array to loacate each single cube
void indexRedefine();

// resolution settings
unsigned int windowWidth = 800, windowHeight = 600;

// camera
Camera camera(glm::vec3(1.0f, 1.0f, 1.0f),
              glm::vec3(0.0f, 1.0f, 0.0f),
              -135, -36);
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// model related data
// ------------------

// model vertex data
const float halfSideLen = 0.1f;
const float sideLen = halfSideLen * 2;
const float singleCubeVertices[] = {
    //              Position                        Color          Texture
    // back face
    -halfSideLen, -halfSideLen, -halfSideLen, 0.0f, 1.0f, 0.0f, 0.333333f, 0.0f,
    -halfSideLen, halfSideLen, -halfSideLen, 0.0f, 1.0f, 0.0f, 0.333333f, 0.5f,
    halfSideLen, halfSideLen, -halfSideLen, 0.0f, 1.0f, 0.0f, 0.0, 0.5f,
    halfSideLen, halfSideLen, -halfSideLen, 0.0f, 1.0f, 0.0f, 0.0, 0.5f,
    halfSideLen, -halfSideLen, -halfSideLen, 0.0f, 1.0f, 0.0f, 0.0, 0.0f,
    -halfSideLen, -halfSideLen, -halfSideLen, 0.0f, 1.0f, 0.0f, 0.333333f, 0.0f,
    // front face
    -halfSideLen, -halfSideLen, halfSideLen, 0.0f, 0.0f, 1.0f, 0.333333f, 0.0f,
    halfSideLen, -halfSideLen, halfSideLen, 0.0f, 0.0f, 1.0f, 0.666667f, 0.0f,
    halfSideLen, halfSideLen, halfSideLen, 0.0f, 0.0f, 1.0f, 0.666667f, 0.5f,
    halfSideLen, halfSideLen, halfSideLen, 0.0f, 0.0f, 1.0f, 0.666667f, 0.5f,
    -halfSideLen, halfSideLen, halfSideLen, 0.0f, 0.0f, 1.0f, 0.333333f, 0.5f,
    -halfSideLen, -halfSideLen, halfSideLen, 0.0f, 0.0f, 1.0f, 0.333333f, 0.0f,
    // left face
    -halfSideLen, halfSideLen, halfSideLen, 1.0f, 0.5f, 0.0f, 1.0f, 0.5f,
    -halfSideLen, halfSideLen, -halfSideLen, 1.0f, 0.5f, 0.0f, 0.666667f, 0.5f,
    -halfSideLen, -halfSideLen, -halfSideLen, 1.0f, 0.5f, 0.0f, 0.666667f, 0.0f,
    -halfSideLen, -halfSideLen, -halfSideLen, 1.0f, 0.5f, 0.0f, 0.666667f, 0.0f,
    -halfSideLen, -halfSideLen, halfSideLen, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,
    -halfSideLen, halfSideLen, halfSideLen, 1.0f, 0.5f, 0.0f, 1.0f, 0.5f,
    // right face
    halfSideLen, halfSideLen, halfSideLen, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    halfSideLen, -halfSideLen, halfSideLen, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f,
    halfSideLen, -halfSideLen, -halfSideLen, 1.0f, 0.0f, 0.0f, 0.333333f, 0.5f,
    halfSideLen, -halfSideLen, -halfSideLen, 1.0f, 0.0f, 0.0f, 0.333333f, 0.5f,
    halfSideLen, halfSideLen, -halfSideLen, 1.0f, 0.0f, 0.0f, 0.333333f, 1.0f,
    halfSideLen, halfSideLen, halfSideLen, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    // bottom face
    -halfSideLen, -halfSideLen, -halfSideLen, 1.0f, 1.0f, 1.0f, 0.333333f, 0.5f,
    halfSideLen, -halfSideLen, -halfSideLen, 1.0f, 1.0f, 1.0f, 0.666667f, 0.5f,
    halfSideLen, -halfSideLen, halfSideLen, 1.0f, 1.0f, 1.0f, 0.666667f, 1.0f,
    halfSideLen, -halfSideLen, halfSideLen, 1.0f, 1.0f, 1.0f, 0.666667f, 1.0f,
    -halfSideLen, -halfSideLen, halfSideLen, 1.0f, 1.0f, 1.0f, 0.333333f, 1.0f,
    -halfSideLen, -halfSideLen, -halfSideLen, 1.0f, 1.0f, 1.0f, 0.333333f, 0.5f,
    // top face
    -halfSideLen, halfSideLen, -halfSideLen, 1.0f, 1.0f, 0.0f, 0.666667f, 1.0f,
    -halfSideLen, halfSideLen, halfSideLen, 1.0f, 1.0f, 0.0f, 0.666667f, 0.5f,
    halfSideLen, halfSideLen, halfSideLen, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f,
    halfSideLen, halfSideLen, halfSideLen, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f,
    halfSideLen, halfSideLen, -halfSideLen, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -halfSideLen, halfSideLen, -halfSideLen, 1.0f, 1.0f, 0.0f, 0.666667f, 1.0f};

// model matrixs
glm::mat4 cubeModel[27];
int cubeIndex[3][3][3] = {0};

// model movement
const float ANGULAR_SPEED = 90.0f;

// world space positions of our cubes
const glm::vec3 cubeOriginPositions[] = {
    // layer 0
    // // slice 0
    glm::vec3(-sideLen, -sideLen, -sideLen),
    glm::vec3(0.0f, -sideLen, -sideLen),
    glm::vec3(sideLen, -sideLen, -sideLen),
    // // slice 1
    glm::vec3(-sideLen, -sideLen, 0.0f),
    glm::vec3(0.0f, -sideLen, 0.0f),
    glm::vec3(sideLen, -sideLen, 0.0f),
    // // slice 2
    glm::vec3(-sideLen, -sideLen, sideLen),
    glm::vec3(0.0f, -sideLen, sideLen),
    glm::vec3(sideLen, -sideLen, sideLen),
    // layer 1
    // // slice 0
    glm::vec3(-sideLen, 0.0f, -sideLen),
    glm::vec3(0.0f, 0.0f, -sideLen),
    glm::vec3(sideLen, 0.0f, -sideLen),
    // // slice 1
    glm::vec3(-sideLen, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(sideLen, 0.0f, 0.0f),
    // // slice 2
    glm::vec3(-sideLen, 0.0f, sideLen),
    glm::vec3(0.0f, 0.0f, sideLen),
    glm::vec3(sideLen, 0.0f, sideLen),
    // layer 2
    // // slice 0
    glm::vec3(-sideLen, sideLen, -sideLen),
    glm::vec3(0.0f, sideLen, -sideLen),
    glm::vec3(sideLen, sideLen, -sideLen),
    // // slice 1
    glm::vec3(-sideLen, sideLen, 0.0f),
    glm::vec3(0.0f, sideLen, 0.0f),
    glm::vec3(sideLen, sideLen, 0.0f),
    // // slice 2
    glm::vec3(-sideLen, sideLen, sideLen),
    glm::vec3(0.0f, sideLen, sideLen),
    glm::vec3(sideLen, sideLen, sideLen),
};

// status vars
enum editSection
{
    NONE = 0,
    Y_BOTTOM_SECTION = 1,
    Y_MIDDLE_SECTION = 2,
    Y_TOP_SECTION = 3,
    Z_BACK_SECTION = 4,
    Z_MIDDLE_SECTION = 8,
    Z_FRONT_SECTION = 12,
    X_LEFT_SECTION = 16,
    X_MIDDLE_SECTION = 32,
    X_RIGHT_SECTION = 48
} nowEditing;
enum rotateDirection
{
    STOP = 0,
    CLOCK = 1,
    CONTC = -1
} nowRotate;
bool textureMode = true;

int main()
{
    std::ios::sync_with_stdio(false);

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
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // build and compile shader program (texture shader & color shader)
    // ------------------------------------
    Shader textureShader("./resource/shader/vertexShader.glsl",
                         "./resource/shader/fragmentShader.glsl"),
        colorShader("./resource/shader/vertexShaderColor.glsl",
                    "./resource/shader/fragmentShaderColor.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind vertex array obj to store ptr to vbo and attribs of vbo in graphic memory
    glBindVertexArray(VAO);

    // bind vertex buffer obj to store vertex data in graphic memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(singleCubeVertices), singleCubeVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load six texture for each face
    // -------------------------
    GLuint cubeTexture[27];
    byte *textureSource[6];
    bool flag = true; // texture load success flag bit
    glGenTextures(27, cubeTexture);
    for (int i = 0; i < 6; ++i)
    {
        std::stringstream path;
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        path << "./resource/texture/"
             << "cube" << ((i < 10) ? "0" : "") << i << ".png";
        textureSource[i] = stbi_load(path.str().c_str(), &width, &height, &nrChannels, 0);
        if (!textureSource[i])
        {
            std::cerr << "Failed to load texture " << path.str() << std::endl;
            flag = false;
        }
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    textureShader.use();
    textureShader.setInt("Texture", 0);

    // gen origin index &
    // gen origin model matrix &
    // split source texture & create cube texture
    // -------------------------------------------------
    for (int j = 0; j < 3; ++j)
    {
        for (int k = 0; k < 3; ++k)
        {
            for (int i = 0; i < 3; ++i)
            {
                cubeIndex[i][j][k] = i + k * 3 + j * 3 * 3;
                cubeModel[cubeIndex[i][j][k]] =
                    glm::translate(glm::mat4(1.0f), ((glm::vec3 *)cubeOriginPositions)[cubeIndex[i][j][k]]);

                glBindTexture(GL_TEXTURE_2D, cubeTexture[cubeIndex[i][j][k]]);

                // set the texture wrapping parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                // set texture filtering parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                byte *tmp = (byte *)malloc(1536 * 1024 * 4);
                memset(tmp, 0xcd, 512 * 4 * 512 * 6);

                // split source image into each cubes' texture
                // (in a very rude way)
                // TODO: make this better! (more format support, more safe, etc.)
                if (flag)
                {
                    // back face
                    if (k == 0)
                    {
                        for (int u = 0; u < 512; ++u)
                        {
                            memcpy(tmp + u * 1536 * 4,
                                   textureSource[0] + j * 1536 * 4 * 512 + (2 - i) * 512 * 4 + u * 1536 * 4,
                                   512 * 4);
                        }
                    }

                    // front face
                    if (k == 2)
                    {
                        for (int u = 0; u < 512; ++u)
                        {
                            memcpy(tmp + 512 * 4 + u * 1536 * 4,
                                   textureSource[1] + j * 1536 * 4 * 512 + i * 512 * 4 + u * 1536 * 4,
                                   512 * 4);
                        }
                    }

                    // left face
                    if (i == 0)
                    {
                        for (int u = 0; u < 512; ++u)
                        {
                            memcpy(tmp + 512 * 2 * 4 + u * 1536 * 4,
                                   textureSource[2] + j * 1536 * 4 * 512 + k * 512 * 4 + u * 1536 * 4,
                                   512 * 4);
                        }
                    }

                    // right face
                    if (i == 2)
                    {
                        for (int u = 0; u < 512; ++u)
                        {
                            memcpy(tmp + 1536 * 4 * 512 + u * 1536 * 4,
                                   textureSource[3] + j * 1536 * 4 * 512 + (2 - k) * 512 * 4 + u * 1536 * 4,
                                   512 * 4);
                        }
                    }

                    // bottom face
                    if (j == 0)
                    {
                        for (int u = 0; u < 512; ++u)
                        {
                            memcpy(tmp + 1536 * 4 * 512 + 512 * 4 + u * 1536 * 4,
                                   textureSource[4] + k * 1536 * 4 * 512 + i * 512 * 4 + u * 1536 * 4,
                                   512 * 4);
                        }
                    }

                    // top face
                    if (j == 2)
                    {
                        for (int u = 0; u < 512; ++u)
                        {
                            memcpy(tmp + 1536 * 4 * 512 + 512 * 2 * 4 + u * 1536 * 4,
                                   textureSource[5] + (2 - k) * 1536 * 4 * 512 + i * 512 * 4 + u * 1536 * 4,
                                   512 * 4);
                        }
                    }
                }

                // transfer texture to graphic mem
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1536, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp);

                // gen mipmap for scaling
                glGenerateMipmap(GL_TEXTURE_2D);

                // free temporary texture in system memory
                free(tmp);
            }
        }
    }

    // free source image in system memory
    for (int i = 0; i < 6; ++i)
        stbi_image_free(textureSource[i]);

    nowEditing = NONE;
    nowRotate = STOP;
    float angle = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // TODO: Find a efficient way to calculate & display FPS
        //std::cout << "FPS:" << 1 / deltaTime << std::endl;
        //std::cout << camera.Yaw << " " << camera.Pitch << std::endl;

        // input
        // -----
        processInput(window);

        // render pure color background
        // ----------------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        Shader *nowShader = textureMode ? &textureShader : &colorShader;
        nowShader->use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        nowShader->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        nowShader->setMat4("view", view);

        // render cubes
        glBindVertexArray(VAO);

        glm::vec3 rotateVector = glm::vec3(0.0f);
        // process rotation matrix
        if (nowEditing && nowRotate)
        {
            if (abs(angle += ((float)nowRotate) * (sin(glm::radians(abs(angle)) * 2.0) + 0.1) * ANGULAR_SPEED * deltaTime) > 90.0f)
            {
                angle = 0;
                indexRedefine();
                nowRotate = STOP;
            }
            switch (nowEditing)
            {
            case Y_BOTTOM_SECTION:
            case Y_MIDDLE_SECTION:
            case Y_TOP_SECTION:
                rotateVector = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            case Z_BACK_SECTION:
            case Z_MIDDLE_SECTION:
            case Z_FRONT_SECTION:
                rotateVector = glm::vec3(0.0f, 0.0f, 1.0f);
                break;
            case X_LEFT_SECTION:
            case X_MIDDLE_SECTION:
            case X_RIGHT_SECTION:
                rotateVector = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
            }
        }
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                for (int i = 0; i < 3; ++i)
                {
                    // bind textures on corresponding texture units
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, cubeTexture[cubeIndex[i][j][k]]);

                    // calculate the model matrix for each object and pass it to shader before drawing
                    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                    if (j + 1 == nowEditing || k + 1 == nowEditing >> 2 || i + 1 == nowEditing >> 4)
                    {
                        nowShader->setVec3("mask", glm::vec3(-0.5f, -0.5f, -0.5f));
                        model = cubeModel[cubeIndex[i][j][k]];
                        if (nowRotate)
                            model = glm::rotate(glm::mat4(1.0f), (float)glm::radians(angle), rotateVector) * cubeModel[cubeIndex[i][j][k]];
                    }
                    else
                    {
                        nowShader->setVec3("mask", glm::vec3(0.0f, 0.0f, 0.0f));
                        model = cubeModel[cubeIndex[i][j][k]];
                    }

                    nowShader->setMat4("model", model);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    if (!textureMode)
                        glDrawArrays(GL_LINE, 0, 36);
                }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(27, cubeTexture);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process camera input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    float originSpeed = camera.MovementSpeed;

    // Camera control
    // --------------

    // LShift for camera speed up
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.MovementSpeed = originSpeed * 4;

    // WASD for camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    camera.MovementSpeed = originSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

//
// -----------
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // Workflow control
    // ----------------

    // ESC for exiting program
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, true);

    // T for switching between
    if (action == GLFW_PRESS && key == GLFW_KEY_T)
        textureMode = !textureMode;

    // Magic Cube Manual Rotation
    // -------------------------------------

    // Num Pad Number for section activation
    // X-axis
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_1 && !nowRotate)
        nowEditing = X_LEFT_SECTION;
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_2 && !nowRotate)
        nowEditing = X_MIDDLE_SECTION;
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_3 && !nowRotate)
        nowEditing = X_RIGHT_SECTION;

    // Y-axis
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_4 && !nowRotate)
        nowEditing = Y_BOTTOM_SECTION;
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_5 && !nowRotate)
        nowEditing = Y_MIDDLE_SECTION;
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_6 && !nowRotate)
        nowEditing = Y_TOP_SECTION;

    // Z-axis
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_7 && !nowRotate)
        nowEditing = Z_BACK_SECTION;
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_8 && !nowRotate)
        nowEditing = Z_MIDDLE_SECTION;
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_9 && !nowRotate)
        nowEditing = Z_FRONT_SECTION;

    // Deactivate manual editing
    if (action == GLFW_PRESS && key == GLFW_KEY_KP_0 && !nowRotate)
        nowEditing = NONE;

    // Q/E for section routation direction
    if (action == GLFW_PRESS && key == GLFW_KEY_LEFT_BRACKET && nowEditing)
        nowRotate = CONTC;
    if (action == GLFW_PRESS && key == GLFW_KEY_RIGHT_BRACKET && nowEditing)
        nowRotate = CLOCK;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
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
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// update index by 90 degree rotation
// ----------------------------------
void indexRedefine()
{
    int originLayIndex[3][3], editingStage;

    // Brute-force, not elegant
    // TODO: use matrix tras & mirr to rotate 90 degree
    switch (nowEditing)
    {
    case Y_BOTTOM_SECTION:
    case Y_MIDDLE_SECTION:
    case Y_TOP_SECTION:
        editingStage = (nowEditing & 3) - 1;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                originLayIndex[i][j] = cubeIndex[i][editingStage][j];
                cubeModel[cubeIndex[i][editingStage][j]] = glm::rotate(glm::mat4(1.0f), nowRotate * (float)glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * cubeModel[cubeIndex[i][editingStage][j]];
            }
        if (nowRotate == CLOCK)
        {
            cubeIndex[0][editingStage][0] = originLayIndex[2][0];
            cubeIndex[0][editingStage][1] = originLayIndex[1][0];
            cubeIndex[0][editingStage][2] = originLayIndex[0][0];
            cubeIndex[1][editingStage][0] = originLayIndex[2][1];
            cubeIndex[1][editingStage][2] = originLayIndex[0][1];
            cubeIndex[2][editingStage][0] = originLayIndex[2][2];
            cubeIndex[2][editingStage][1] = originLayIndex[1][2];
            cubeIndex[2][editingStage][2] = originLayIndex[0][2];
        }
        else
        {
            cubeIndex[0][editingStage][0] = originLayIndex[0][2];
            cubeIndex[0][editingStage][1] = originLayIndex[1][2];
            cubeIndex[0][editingStage][2] = originLayIndex[2][2];
            cubeIndex[1][editingStage][0] = originLayIndex[0][1];
            cubeIndex[1][editingStage][2] = originLayIndex[2][1];
            cubeIndex[2][editingStage][0] = originLayIndex[0][0];
            cubeIndex[2][editingStage][1] = originLayIndex[1][0];
            cubeIndex[2][editingStage][2] = originLayIndex[2][0];
        }
        break;
    case Z_BACK_SECTION:
    case Z_MIDDLE_SECTION:
    case Z_FRONT_SECTION:
        editingStage = ((nowEditing >> 2) & 3) - 1;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                originLayIndex[i][j] = cubeIndex[i][j][editingStage];
                cubeModel[cubeIndex[i][j][editingStage]] = glm::rotate(glm::mat4(1.0f), nowRotate * (float)glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * cubeModel[cubeIndex[i][j][editingStage]];
            }
        if (nowRotate == CLOCK)
        {
            cubeIndex[0][0][editingStage] = originLayIndex[0][2];
            cubeIndex[0][1][editingStage] = originLayIndex[1][2];
            cubeIndex[0][2][editingStage] = originLayIndex[2][2];
            cubeIndex[1][0][editingStage] = originLayIndex[0][1];
            cubeIndex[1][2][editingStage] = originLayIndex[2][1];
            cubeIndex[2][0][editingStage] = originLayIndex[0][0];
            cubeIndex[2][1][editingStage] = originLayIndex[1][0];
            cubeIndex[2][2][editingStage] = originLayIndex[2][0];
        }
        else
        {
            cubeIndex[0][0][editingStage] = originLayIndex[2][0];
            cubeIndex[0][1][editingStage] = originLayIndex[1][0];
            cubeIndex[0][2][editingStage] = originLayIndex[0][0];
            cubeIndex[1][0][editingStage] = originLayIndex[2][1];
            cubeIndex[1][2][editingStage] = originLayIndex[0][1];
            cubeIndex[2][0][editingStage] = originLayIndex[2][2];
            cubeIndex[2][1][editingStage] = originLayIndex[1][2];
            cubeIndex[2][2][editingStage] = originLayIndex[0][2];
        }
        break;
    case X_LEFT_SECTION:
    case X_MIDDLE_SECTION:
    case X_RIGHT_SECTION:
        editingStage = ((nowEditing >> 4) & 3) - 1;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
            {
                originLayIndex[i][j] = cubeIndex[editingStage][i][j];
                cubeModel[cubeIndex[editingStage][i][j]] = glm::rotate(glm::mat4(1.0f), nowRotate * (float)glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * cubeModel[cubeIndex[editingStage][i][j]];
            }
        if (nowRotate == CLOCK)
        {
            cubeIndex[editingStage][0][0] = originLayIndex[0][2];
            cubeIndex[editingStage][0][1] = originLayIndex[1][2];
            cubeIndex[editingStage][0][2] = originLayIndex[2][2];
            cubeIndex[editingStage][1][0] = originLayIndex[0][1];
            cubeIndex[editingStage][1][2] = originLayIndex[2][1];
            cubeIndex[editingStage][2][0] = originLayIndex[0][0];
            cubeIndex[editingStage][2][1] = originLayIndex[1][0];
            cubeIndex[editingStage][2][2] = originLayIndex[2][0];
        }
        else
        {
            cubeIndex[editingStage][0][0] = originLayIndex[2][0];
            cubeIndex[editingStage][0][1] = originLayIndex[1][0];
            cubeIndex[editingStage][0][2] = originLayIndex[0][0];
            cubeIndex[editingStage][1][0] = originLayIndex[2][1];
            cubeIndex[editingStage][1][2] = originLayIndex[0][1];
            cubeIndex[editingStage][2][0] = originLayIndex[2][2];
            cubeIndex[editingStage][2][1] = originLayIndex[1][2];
            cubeIndex[editingStage][2][2] = originLayIndex[0][2];
        }
        break;
    default:
        break;
    }
}