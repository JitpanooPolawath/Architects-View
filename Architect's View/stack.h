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
	glm::mat4* MS = new glm::mat4[1];
	int counter = 0;
	bool twicePop = false;
public:
	
	void push(glm::mat4 model) {
		int n = sizeof(*MS) / sizeof(MS[0]);
		if (counter == n) {
			glm::mat4* temp = new glm::mat4 [n+1];
			for (int i = 0; i < n; i++) {
				temp[i] = MS[i];
			}
			delete[] MS;
			MS = temp;
		}
		MS[counter] = model;
		counter++;
		twicePop = false;

	}

	glm::mat4 pop() {
		if (counter == 0) {
			return MS[counter];
		}
		counter--;
		if (twicePop == true) {
			glm::mat4* deleteTemp = new glm::mat4[counter];
			for (int i = 0; i < counter; i++) {
				deleteTemp[i] = MS[i];
			}
			delete[] MS;
			MS = deleteTemp;
		}
		twicePop = true;
		return MS[counter];
	}

	void print() {
		for (int i = 0; i < counter; i++) {
			std::cout << glm::to_string(MS[i]) << std::endl;
		}
	}

};