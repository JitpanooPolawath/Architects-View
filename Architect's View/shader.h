#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
private:
    void setModel(glm::mat4 model) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    }

    void setView(glm::mat4 view) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    }


public: 
	unsigned int ID;

	// read vertex and fragment shader from file
	Shader(const char* vertexPath, const char* fragmentPath){

		// Get the vertex and fragment source codes from specified file paths
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// Ensure file streams can signal errors via exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // Attempt to open shader source files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            // Read file contents
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // Close the file streams
            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

        }
        catch (const std::ifstream::failure& streamException) // Catching the specific exception type
        {
            std::cerr << "!! SHADER FILE READ ERROR !! Problem reading one of the source files." << streamException.what() << std::endl;
        }

        // Obtain C-style string pointers from the std::strings. These are needed for glShaderSource
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        // Create and compile the Vertex Shader component
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);

        // Create and compile the Fragment Shader component
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        // Create the main shader program handle and link the compiled stages
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        // Delete the individual shader objects now that they are successfully linked into the program
        glDeleteShader(vertex);
        glDeleteShader(fragment);

	}

    void use() const {
        glUseProgram(ID);
    }

    void setProj(glm::mat4 projection) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    }

    void setColor(glm::vec3 c) {
        glUniform3fv(glGetUniformLocation(ID, "objectColor"), 1, glm::value_ptr(c));
    }

    void setLightColor(glm::vec3 c) {
        glUniform3fv(glGetUniformLocation(ID, "lightColor"), 1, glm::value_ptr(c));
    }

    void setLightPos(glm::vec3 c) {
        glUniform3fv(glGetUniformLocation(ID, "lightPos"), 1, glm::value_ptr(c));
    }

    void setViewPos(glm::vec3 c) {
        glUniform3fv(glGetUniformLocation(ID, "viewPos"), 1, glm::value_ptr(c));
    }


    void setInt(const std::string& name, int value) const {
        if (value == 1) {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), 1);
        }
        else {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), 0);
        }
        
    }

    void setMV(glm::mat4 model, glm::mat4 view) {
        setModel(model);
        setView(view);
    }



};
#endif
