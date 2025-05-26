#pragma once
// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/string_cast.hpp>

#include <iostream>
#include <string>

class MatrixStack {
private:
    glm::mat4* MS;       
    int count;           
    int capacity;       
    static const int INITIAL_CAPACITY = 4; 

    void resize(int newCapacity) {
        if (newCapacity < count) { 
            newCapacity = count > 0 ? count : INITIAL_CAPACITY;
        }
        if (newCapacity < INITIAL_CAPACITY && newCapacity != 0) {
            newCapacity = INITIAL_CAPACITY;
        }


        glm::mat4* temp = new glm::mat4[newCapacity];
        for (int i = 0; i < count; i++) {
            temp[i] = MS[i];
        }
        delete[] MS;
        MS = temp;
        capacity = newCapacity;
    }

public:
    MatrixStack(int initialCapacity = INITIAL_CAPACITY) : count(0) {
        if (initialCapacity <= 0) {
            this->capacity = INITIAL_CAPACITY; 
        }
        else {
            this->capacity = initialCapacity;
        }
        MS = new glm::mat4[this->capacity];
    }

    ~MatrixStack() {
        delete[] MS;
    }

    MatrixStack(const MatrixStack&) = delete;
    MatrixStack& operator=(const MatrixStack&) = delete;


    void push(const glm::mat4& model) {
        if (count == capacity) {
            int newCapacity = (capacity == 0) ? INITIAL_CAPACITY : capacity * 2;
            resize(newCapacity);
        }
        MS[count] = model;
        count++;
    }

    glm::mat4 pop() {
        if (count == 0) {
            throw std::out_of_range("Cannot pop from an empty stack.");

        }

        count--; 
        glm::mat4 topMatrix = MS[count];

        if (capacity > INITIAL_CAPACITY && count > 0 && count <= capacity / 4) {
            int newCapacity = capacity / 2;
            if (newCapacity < INITIAL_CAPACITY) {
                newCapacity = INITIAL_CAPACITY;
            }
            resize(newCapacity);
        }
        return topMatrix;
    }

    const glm::mat4& top() const {
        if (count == 0) {
            throw std::out_of_range("Cannot get top from an empty stack.");
        }
        return MS[count - 1];
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }

    void print() const { // Make print const
        std::cout << "Matrix Stack (Count: " << count << ", Capacity: " << capacity << "):" << std::endl;
        if (count == 0) {
            std::cout << "  <empty>" << std::endl;
            return;
        }
        for (int i = 0; i < count; i++) { // Iterate up to count (actual elements)
            std::cout << "  [" << i << "]: " << glm::to_string(MS[i]) << std::endl;
        }
    }
};