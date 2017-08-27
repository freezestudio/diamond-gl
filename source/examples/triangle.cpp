#include <stdio.h>
#include <GLFW/glfw3.h>
#include <include/diamond/all.hpp>
#include <iostream>

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;


std::string vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
std::string fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glewInit();


    // create shaders
    dgl::shader frag(GL_FRAGMENT_SHADER);
    frag.source(fragmentShaderSource);
    frag.compile();
    if (!frag.get_val<int>(GL_COMPILE_STATUS)) {
        std::cerr << frag.info_log() << std::endl;
    }

    dgl::shader vert(GL_VERTEX_SHADER);
    vert.source(vertexShaderSource);
    vert.compile();
    if (!vert.get_val<int>(GL_COMPILE_STATUS)) {
        std::cerr << vert.info_log() << std::endl;
    }

    dgl::program program;
    program.attach(vert);
    program.attach(frag);
    program.link();
    if (!program.get_val<int>(GL_LINK_STATUS)) {
        std::cerr << program.info_log() << std::endl;
    }

    // create vertices and buffer
    dgl::structured_buffer<glm::vec3> vbo;
    vbo.data(std::vector<glm::vec3>{{
        {-0.5f, -0.5f, 0.0f }, // left  
        { 0.5f, -0.5f, 0.0f }, // right 
        { 0.0f,  0.5f, 0.0f }  // top   
    }});



    // create VAO
    dgl::vertex_array vao;

    auto binding = vao.create_binding(0);
    binding.vertex_buffer(vbo, 0);

    auto attribute = vao.create_attribute(0);
    attribute.attrib_format(3, GL_FLOAT, GL_FALSE);
    attribute.binding(binding);

    
    // program uniform constant example
    auto uniform = program.get_uniform<int>(0);
    uniform = 0;

    // SSBO binding example 
    dgl::buffer_binding ssbo_binding(dgl::buffer_target::shader_storage, 0);
    ssbo_binding.bind(vbo);


    // create texture
    dgl::texture texture(dgl::texture_target::t_2d);
    texture.storage(1, GL_RGBA32F, glm::uvec2(1, 1)); // 2d texture
    texture.parameter_val<int>(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture.parameter_val<int>(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture.parameter_val<int>(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.parameter_val<int>(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // create GL texture binding
    auto textureBinding = dgl::texture_binding(0);
    textureBinding.bind_texture(texture);
    

    // drawing
    while (!glfwWindowShouldClose(window))
    {
        // managment
        dgl::managment::program::use(program);
        dgl::managment::vertex_array::bind(vao);

        // states
        dgl::state::clear::color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        dgl::state::clear::depth(1.f);

        // commands
        dgl::commands::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        dgl::commands::draw::arrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}