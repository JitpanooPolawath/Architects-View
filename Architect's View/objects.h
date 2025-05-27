#pragma once

// OpenGL
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/string_cast.hpp>

// Basic
#include <filesystem> 

class Cube {
public:

    int id;
    glm::vec3 translate;
    glm::vec3 scale;
    float rotate;
    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 30.0;

    GLenum format;
    bool textLoaded = false;
    unsigned int diffuseMap, specularMap;
    Cube() {};

    void updateColor(glm::vec3 amb, glm::vec3 dif, glm::vec3 spe) {
        ambient = amb;
        diffuse = dif;
        specular = spe;
    }
    void setTransformation(int ID, glm::vec3 trans, glm::vec3 scal, float rotat) {
        id = ID;
        translate = trans;
        scale = scal;
        rotate = rotat;
    }

    void setTexture(char const* diffuseText, char const* specularText) {
        diffuseMap = loadTexture(diffuseText);
        specularMap = loadTexture(specularText);
        
    }

    unsigned int loadTexture(char const* path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;

            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    void print() {
        std::cout << id << "---" << glm::to_string(translate) << "---" << glm::to_string(scale) << "---" << rotate << std::endl;
    }

};
