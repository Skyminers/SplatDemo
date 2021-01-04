//
// Created by 刘一辰 on 2020/10/24.
//

#include "Shaders.h"

void Shaders::useProgram(){glUseProgram(ID);}

void Shaders::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shaders::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shaders::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shaders::setVec2(const std::string &name, const glm::vec2 &value) const{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shaders::setVec2(const std::string &name, float x, float y) const{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shaders::setVec3(const std::string &name, const glm::vec3 &value) const{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shaders::setVec3(const std::string &name, float x, float y, float z) const{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shaders::setVec4(const std::string &name, const glm::vec4 &value) const{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shaders::setVec4(const std::string &name, float x, float y, float z, float w){
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shaders::setMat2(const std::string &name, const glm::mat2 &mat) const{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shaders::setMat3(const std::string &name, const glm::mat3 &mat) const{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Shaders::setMat4(const std::string &name, const glm::mat4 &mat) const{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

#define READ_FILE

Shaders::Shaders(const char* vertexShaderFile, const char* fragmentShaderFile){
#ifdef READ_FILE
    std::string vertexSource;
    std::string fragmentSource;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        vShaderFile.open(vertexShaderFile);
        fShaderFile.open(fragmentShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();

        vertexSource = vShaderStream.str();
        fragmentSource = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR when open shaders " << std::endl;
    }
    const char* vShaderSource = vertexSource.c_str();
    const char* fShaderSource = fragmentSource.c_str();
#else
    static const char * vShaderSource = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "layout (location = 1) in vec3 aColor;\n"
                                        "\n"
                                        "out vec3 ourColor;\n"
                                        "\n"
                                        "uniform mat4 model;\n"
                                        "uniform mat4 view;\n"
                                        "uniform mat4 projection;\n"
                                        "uniform bool sun;\n"
                                        "\n"
                                        "void main() {\n"
                                        "    gl_Position = projection * view * model * vec4(aPos, 1);\n"
                                        "    if(sun) ourColor = vec3(0.5f, 1.0f, 0);\n"
                                        "    else ourColor = aColor;\n"
                                        "}\0";
    static const char * fShaderSource = "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "in vec3 ourColor;\n"
                                        "\n"
                                        "void main() {\n"
                                        "    FragColor = vec4(ourColor, 1.0);//vec4(1.0, 0.5, 0.2, 1.0);\n"
                                        "}\0";
#endif
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shaders::~Shaders() {
    glDeleteProgram(ID);
}

void Shaders::checkCompileErrors(GLuint shader, std::string type){
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR when compiling " << type << std::endl << infoLog << std::endl;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR when compiling " << type << std::endl << infoLog << std::endl;
        }
    }
}