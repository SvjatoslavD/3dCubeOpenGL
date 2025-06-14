#include <GL/glew.h>
#include <SFML/Window.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "include/shader.h"

// ---- code is primarily modeled after code found on learnOpenGL.com, but modified to be used within an SFML context ----

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

    // ---- Load resources, initialize the OpenGL states ----

    // activate the window and set the viewport of opengl to be the same as window
    window.setActive();
    glViewport(0,0,win_width,win_height);

    // load glew AFTER context has been created
    glewExperimental = GL_TRUE;
    glewInit();

    // ---- information ----

    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // ---- set up vertex data (and buffer(s)) and configure vertex attributes ----

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
   };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
        // handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {window.close();}
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                // adjust the viewport when the window is resized
                glViewport(0, 0, resized->size.x, resized->size.y);
            }
        }

        // process input

        // update

        // clear the buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE);
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // ---- optional: de-allocate all resources once they've outlived their purpose ----
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}
