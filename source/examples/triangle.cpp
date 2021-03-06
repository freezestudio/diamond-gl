#include <stdio.h>
#include <GLFW/glfw3.h>
#include <include/diamond/all.hpp>
#include <iostream>
#include <tuple>
#include <utility>

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
"   FragColor = vec4(0.2f, 1.0f, 0.5f, 1.0f);\n"
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

    // create buffer
    //dgl::buffer vbo;
    //dgl::buffer another = vbo;
    std::vector<dgl::buffer> vbos = dgl::buffer::create(2);
    //dgl::buffer vbos;
    dgl::buffer vbo = vbos[0];

    //dgl::buffer vbo;
    //vbo.storage(3 * sizeof(glm::vec3));
    vbo.storage(3 * sizeof(glm::vec3));
    vbo.subdata<glm::vec3>(0, {{
        { -0.5f, -0.5f, 0.0f }, // left  
        { 0.5f, -0.5f, 0.0f }, // right 
        { 0.0f,  0.5f, 0.0f }  // top   
    }});

    // create VAO
    dgl::vertex_array vao;

    auto binding = vao.create_binding<glm::vec3>(0);
    binding.vertex_buffer(vbos);

    auto attribute = vao.create_attribute(0);
    attribute.attrib_format(3, GL_FLOAT, GL_FALSE);
    attribute.binding(binding);

    // program uniform constant example
    auto uniform = program.get_uniform<int>(0);
    uniform = 0;

    // SSBO binding example 
    dgl::buffer_binding ssbo_binding(dgl::buffer_target::shader_storage, 0);
    ssbo_binding.bind(vbo); // multi-bind


    // create texture
    dgl::texture texture = dgl::texture(dgl::texture_target::texture2d);
    texture.storage(1, GL_RGBA32F, glm::uvec2(1, 1)); // 2d texture
    texture.parameter_val<int>(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture.parameter_val<int>(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture.parameter_val<int>(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.parameter_val<int>(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture.level(0).subimage(glm::ivec2(0), glm::uvec2(1, 1), GL_RGBA, GL_FLOAT, new float[4]{ 0.0f, 0.0f, 0.0f, 1.0f });

    // create GL texture binding
    auto textureBinding = dgl::texture_binding(0);
    textureBinding.bind_texture(texture);
    

    // drawing
    while (!glfwWindowShouldClose(window))
    {
        // managment
        dgl::managment.use_program(program);
        dgl::managment.bind_vertex_array(vao);
        binding.vertex_buffer(vbo, 0);

        // states
        dgl::clear.color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        dgl::clear.depth(1.f);
        
        // commands
        dgl::commands.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        dgl::commands.draw_arrays(dgl::draw_mode::triangles, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}