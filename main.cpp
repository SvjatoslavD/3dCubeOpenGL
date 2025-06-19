#include <GL/glew.h>
#include <SFML/Window.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "include/shader.h"

// ---- code is primarily modeled after code found on learnOpenGL.com, but modified to be used within an SFML context ----

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;
float mouseScrollSpeed = 50.f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float sineBetween(float min, float max, float t);
void processEventsAndInput(sf::Window& window);

int main() {
    // load variables needed for the creation of the window
    int win_width = 800;
    int win_height = 600;

    sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.depthBits = 24;

    // create the window
    sf::Window window(sf::VideoMode({(unsigned)win_width, (unsigned)win_height}), "OpenGL", sf::Style::Default, sf::State::Windowed, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    //window.setMouseCursorVisible(false);

    // ---- Load resources, initialize the OpenGL states ----

    // activate the window and set the viewport of opengl to be the same as window
    window.setActive();
    glViewport(0,0,win_width,win_height);

    // load glew AFTER context has been created
    glewExperimental = GL_TRUE;
    glewInit();

    // tell openGL to use the depth buffer
    glEnable(GL_DEPTH_TEST);

    // ---- information ----

    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // ---- set up vertex data (and buffer(s)) and configure vertex attributes ----

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load texture to be used later
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int img_width, img_height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Most images are 4 byte aligned, so if pixel data only has 3 bytes, there will be a malformed texture
    unsigned char *data = stbi_load("../assets/carpet1.jpg", &img_width, &img_height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    // ---- build and compile our shader program ----

    Shader ourShader("../shaders/shader1.vert", "../shaders/shader1.frag");
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture"), 0);

    sf::Clock clock;

    while (window.isOpen()) {
        float currentFrame =clock.getElapsedTime().asSeconds();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processEventsAndInput(window);

        { // rendering the cubes to the screen
            // clear the buffers
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE);
            glBindTexture(GL_TEXTURE_2D, texture);

            // pass projection matrix to shader (note that in this case it could change every frame)
            glm::mat4 projection = glm::perspective(glm::radians(fov), (float)win_width / (float)win_height, 0.1f, 100.0f);
            ourShader.setMat4("projection", projection);

            // camera/view transformation
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            ourShader.setMat4("view", view);

            // render boxes
            glBindVertexArray(VAO);
            for (unsigned int i = 0; i < 10; i++) {
                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(1.0f);
                glm::vec3 currentCubePos = cubePositions[i];
                model = glm::translate(model, currentCubePos);
                float angle = 100.f + 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(sin(2*clock.getElapsedTime().asSeconds()), 0.3f, 0.5f));
                ourShader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // render container
            ourShader.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // end the current frame (internally swaps the front and back buffers)
            window.display();
        }
    }

    // ---- optional: de-allocate all resources once they've outlived their purpose ----
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

float sineBetween(float min, float max, float t) {
    float halfRange = (max - min) / 2;
    return min + halfRange + sin(t) * halfRange;
}

void processEventsAndInput(sf::Window& window) {
    // handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {window.close();}
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                // adjust the viewport when the window is resized
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                if (isButtonPressed(sf::Mouse::Button::Left)) {
                    float xpos = static_cast<float>(mouseMoved->position.x);
                    float ypos = static_cast<float>(mouseMoved->position.y);

                    if (firstMouse) {
                        lastX = xpos;
                        lastY = ypos;
                        firstMouse = false;
                    }

                    float xoffset = xpos - lastX;
                    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
                    lastX = xpos;
                    lastY = ypos;

                    float sensitivity = 0.1f; // change this value to your liking
                    xoffset *= sensitivity;
                    yoffset *= sensitivity;

                    yaw += xoffset;
                    pitch += yoffset;

                    // make sure that when pitch is out of bounds, screen doesn't get flipped
                    if (pitch > 89.0f)
                        pitch = 89.0f;
                    if (pitch < -89.0f)
                        pitch = -89.0f;

                    glm::vec3 front;
                    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    front.y = sin(glm::radians(pitch));
                    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    cameraFront = glm::normalize(front);
                }
            }
            if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                    firstMouse = true;
                }
            }
            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheel->delta > 0) {
                    fov -= mouseScrollSpeed * deltaTime;
                    if (fov < 1.0f) {fov = 1.0f;}
                }
                if (mouseWheel->delta < 0) {
                    fov += mouseScrollSpeed * deltaTime;
                    if (fov > 55.0f) {fov = 55.0f;}
                }
            }
        }

        // process input
        float cameraSpeed = 4.f * deltaTime;
        if (isKeyPressed(sf::Keyboard::Key::W)) {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (isKeyPressed(sf::Keyboard::Key::S)) {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (isKeyPressed(sf::Keyboard::Key::A)) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (isKeyPressed(sf::Keyboard::Key::D)) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (isKeyPressed(sf::Keyboard::Key::Q)) {window.close();}
}