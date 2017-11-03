#version 150

//User input matrices
uniform mat4 modelMatrix;
uniform mat4 projectorProjectionMatrix;
uniform mat4 projectorViewMatrix;

//GLSL Predefined matrix
uniform mat4 modelViewProjectionMatrix;

//GLSL Predefined variable
in vec4 position;

void main() {
    gl_Position = projectorProjectionMatrix * projectorViewMatrix * modelMatrix * position;
}

