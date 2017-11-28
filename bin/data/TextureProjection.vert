#version 150


//Shared variables
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 biasMatrix;
in vec4 position;
in vec3 normal;
out vec3 normalVarying;

//projector's variables
uniform mat4 projectorMatrix;
uniform mat4 projectorProjectionMatrix;
uniform mat4 projectorViewMatrix;
uniform vec3 projectorPos;
out vec4 projTextCoord;
out vec4 shadowTextCoord;
out vec3 projectorDirVarying;
out float distVarying;
out vec3 halfVector;
void setProjector() {
    projTextCoord = projectorMatrix * modelMatrix * position;
    shadowTextCoord = (biasMatrix * projectorProjectionMatrix * projectorViewMatrix) * modelMatrix * position;
    
    vec3 aux = projectorPos - vec3(modelMatrix * position);
    projectorDirVarying = normalize(aux);
    distVarying = length(aux);
    
    vec3 L = projectorPos - position.xyz;
    vec3 V = -position.xyz;
    halfVector = normalize(L+V);
}

void main() {
    setProjector();
    
    gl_Position = modelViewProjectionMatrix * position;
    
    normalVarying = normalize(normal);
}
