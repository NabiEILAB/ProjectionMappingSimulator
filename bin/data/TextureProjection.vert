#version 150

//Shared variables
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;
uniform mat4 biasMatrix;
in vec4 position;
in vec3 normal;
out vec3 normalVarying;

//first projector's variables
uniform mat4 projectorMatrix1;
uniform mat4 projectorProjectionMatrix1;
uniform mat4 projectorViewMatrix1;
uniform vec3 projectorPos1;
out vec4 projTextCoord1;
out vec4 shadowTextCoord1;
out vec3 projectorDirVarying1;
out float distVarying1;
out vec3 halfVector1;
void setProjector1() {
    projTextCoord1 = projectorMatrix1 * modelMatrix * position;
    shadowTextCoord1 = (biasMatrix * projectorProjectionMatrix1 * projectorViewMatrix1) * modelMatrix * position;
    
    vec3 aux = projectorPos1 - vec3(modelMatrix * position);
    projectorDirVarying1 = normalize(aux);
    distVarying1 = length(aux);
    
    vec3 L = projectorPos1 - position.xyz;
    vec3 V = -position.xyz;
    halfVector1 = normalize(L+V);
}

//second projector's variables
uniform mat4 projectorMatrix2;
uniform mat4 projectorProjectionMatrix2;
uniform mat4 projectorViewMatrix2;
uniform vec3 projectorPos2;
out vec4 projTextCoord2;
out vec4 shadowTextCoord2;
out vec3 projectorDirVarying2;
out float distVarying2;
out vec3 halfVector2;
void setProjector2() {
    projTextCoord2 = projectorMatrix2 * modelMatrix * position;
    shadowTextCoord2 = (biasMatrix * projectorProjectionMatrix2 * projectorViewMatrix2) * modelMatrix * position;
    
    vec3 aux = projectorPos2 - vec3(modelMatrix * position);
    projectorDirVarying2 = normalize(aux);
    distVarying2 = length(aux);
    
    vec3 L = projectorPos2 - position.xyz;
    vec3 V = -position.xyz;
    halfVector2 = normalize(L+V);
}

//third... fourth...


void main() {
    setProjector1();
    setProjector2();
    
    gl_Position = modelViewProjectionMatrix * position;
    
    normalVarying = normalize(normal);
}
