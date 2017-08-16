#version 150

//GLSL predefined matrices
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;

//User input matrices
uniform mat4 modelMatrix;
uniform mat4 projectorMatrix;
uniform mat4 projectorProjectionMatrix;
uniform mat4 projectorViewMatrix;
uniform mat4 biasMatrix;

//User input variable
uniform vec3 projectorPos;

//GLSL predefined variables
in vec4 position;
in vec3 normal;

//Varying variables
out vec4 projTextCoord;
out vec4 shadowTextCoord;
out vec3 projectorPosVarying;
out vec3 positionVarying;
out vec3 normalVarying;

void main() {
    projTextCoord = projectorMatrix * modelMatrix * position;
    //shadowTextCoord = (biasMatrix * projectorProjectionMatrix * projectorViewMatrix * inverse(viewMatrix)) * viewMatrix * modelMatrix * position;
    shadowTextCoord = (biasMatrix * projectorProjectionMatrix * projectorViewMatrix) * modelMatrix * position;
    gl_Position = modelViewProjectionMatrix * position;
    
    projectorPosVarying = projectorPos;
    positionVarying = vec3(modelMatrix * position);
    normalVarying = normal;
}
