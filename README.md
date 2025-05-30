# Architect's View

A program that allows users to take a photo of a building blueprint and render it using OpenGL. 

# Main features
- Generate a 3D building
-   Algorithm 
- Move and interact with the camera - completed
- Change lighting behaviour (single lighting)
-   Basic lighting (ADS) - Completed
-   Ability to move the light with the camera - Completed
-   Ability to put light in a specific coordinate - Completed (maybe need an input text box)
-   Ability to change light color/brightness - Completed
- Read from image (Python and output as a file for C++ to read) - Completed
- GUI - sort of complete
- Write a Python script that executes the C++ file after OCR

# Usage
1 - Build the Architect's View C++ file
2 - Move the vertex shader file, fragment shader file, texture folder, Architect's View.exe, and main.py file into a directory of your choice and run main.py

# Additional features
- More Complex lighting - shadow, multiple lights, and more
- Normal mapping for walls


# Libraries needed
C++
- GLFW
- GLAD
- GLM
- imGUI

Python 
- EasyOCR

The blueprint must follow specific specifications to work.

