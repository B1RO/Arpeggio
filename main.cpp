#include <iostream>

#include <iostream>

#undef GLFW_DLL

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

using namespace std;

void render(GLFWwindow *win) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glfwSwapBuffers(win);
    glfwPollEvents();
}

static unsigned int compileShader(const std::string &source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    //TODO : Error handling

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "FAILED TO COMPILE SHADER!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}

int main() {
    if (!glfwInit()) {
        cerr << "Can't initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    GLFWwindow *win;
    win = glfwCreateWindow(450, 300, "Hello", NULL, NULL);
    glfwMakeContextCurrent(win);

    auto result = glewInit();
    if (result != GLEW_OK) {
        cerr << "Can't initalize GLEW" << result << endl;
    }

    float positions[8] =
            {
                    -0.5f, -0.5f,
                    0.5f, -0.5f,
                    0.5f, 0.5f,
                    -0.5f, 0.5f
            };
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
    glEnableVertexAttribArray(0);

    unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
    };

    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    std::string vertexShader =
            "#version 330 core\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            "gl_Position = position;\n"
            "}\n";

    std::string fragmentShader =
            "#version 330 core\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "color = vec4(1.0,0.0,0.0,1.0);\n"
            "}\n";

    unsigned int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(win)) {
        render(win);
        glfwWaitEvents();
        glfwSetFramebufferSizeCallback(
                win,
                onResize);
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}



