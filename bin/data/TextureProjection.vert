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
out vec3 fragPos;

//first projector's variables
uniform mat4 projectorMatrix1;
uniform mat4 projectorProjectionMatrix1;
uniform mat4 projectorViewMatrix1;
uniform vec3 projectorPos1;
uniform float isSet1;
out vec4 projTextCoord1;
out vec4 shadowTextCoord1;
out vec3 projectorDirVarying1;
out float distVarying1;
out vec3 halfVector1;
out float isSetVarying1;
void setProjector1() {
    isSetVarying1 = isSet1;
    if(isSet1<=0)
        return ;
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
uniform float isSet2;
out vec4 projTextCoord2;
out vec4 shadowTextCoord2;
out vec3 projectorDirVarying2;
out float distVarying2;
out vec3 halfVector2;
out float isSetVarying2;
void setProjector2() {
    isSetVarying2 = isSet2;
    if(isSet2<=0)
        return ;
    projTextCoord2 = projectorMatrix2 * modelMatrix * position;
    shadowTextCoord2 = (biasMatrix * projectorProjectionMatrix2 * projectorViewMatrix2) * modelMatrix * position;
    
    vec3 aux = projectorPos2 - vec3(modelMatrix * position);
    projectorDirVarying2 = normalize(aux);
    distVarying2 = length(aux);
    
    vec3 L = projectorPos2 - position.xyz;
    vec3 V = -position.xyz;
    halfVector2 = normalize(L+V);
}

//third projector's variables
uniform mat4 projectorMatrix3;
uniform mat4 projectorProjectionMatrix3;
uniform mat4 projectorViewMatrix3;
uniform vec3 projectorPos3;
uniform float isSet3;
out vec4 projTextCoord3;
out vec4 shadowTextCoord3;
out vec3 projectorDirVarying3;
out float distVarying3;
out vec3 halfVector3;
out float isSetVarying3;
void setProjector3() {
    isSetVarying3 = isSet3;
    if(isSet3<=0)
        return ;
    projTextCoord3 = projectorMatrix3 * modelMatrix * position;
    shadowTextCoord3 = (biasMatrix * projectorProjectionMatrix3 * projectorViewMatrix3) * modelMatrix * position;
    
    vec3 aux = projectorPos3 - vec3(modelMatrix * position);
    projectorDirVarying3 = normalize(aux);
    distVarying3 = length(aux);
    
    vec3 L = projectorPos3 - position.xyz;
    vec3 V = -position.xyz;
    halfVector3 = normalize(L+V);
}

//fourth projector's variables
uniform mat4 projectorMatrix4;
uniform mat4 projectorProjectionMatrix4;
uniform mat4 projectorViewMatrix4;
uniform vec3 projectorPos4;
uniform float isSet4;
out vec4 projTextCoord4;
out vec4 shadowTextCoord4;
out vec3 projectorDirVarying4;
out float distVarying4;
out vec3 halfVector4;
out float isSetVarying4;
void setProjector4() {
    isSetVarying4 = isSet4;
    if(isSet4<=0)
        return ;
    projTextCoord4 = projectorMatrix4 * modelMatrix * position;
    shadowTextCoord4 = (biasMatrix * projectorProjectionMatrix4 * projectorViewMatrix4) * modelMatrix * position;
    
    vec3 aux = projectorPos4 - vec3(modelMatrix * position);
    projectorDirVarying4 = normalize(aux);
    distVarying4 = length(aux);
    
    vec3 L = projectorPos4 - position.xyz;
    vec3 V = -position.xyz;
    halfVector4 = normalize(L+V);
}

//fifth projector's variables
uniform mat4 projectorMatrix5;
uniform mat4 projectorProjectionMatrix5;
uniform mat4 projectorViewMatrix5;
uniform vec3 projectorPos5;
uniform float isSet5;
out vec4 projTextCoord5;
out vec4 shadowTextCoord5;
out vec3 projectorDirVarying5;
out float distVarying5;
out vec3 halfVector5;
out float isSetVarying5;
void setProjector5() {
    isSetVarying5 = isSet5;
    if(isSet5<=0)
        return ;
    projTextCoord5 = projectorMatrix5 * modelMatrix * position;
    shadowTextCoord5 = (biasMatrix * projectorProjectionMatrix5 * projectorViewMatrix5) * modelMatrix * position;
    
    vec3 aux = projectorPos5 - vec3(modelMatrix * position);
    projectorDirVarying5 = normalize(aux);
    distVarying5 = length(aux);
    
    vec3 L = projectorPos5 - position.xyz;
    vec3 V = -position.xyz;
    halfVector5 = normalize(L+V);
}

//sixth projector's variables
uniform mat4 projectorMatrix6;
uniform mat4 projectorProjectionMatrix6;
uniform mat4 projectorViewMatrix6;
uniform vec3 projectorPos6;
uniform float isSet6;
out vec4 projTextCoord6;
out vec4 shadowTextCoord6;
out vec3 projectorDirVarying6;
out float distVarying6;
out vec3 halfVector6;
out float isSetVarying6;
void setProjector6() {
    isSetVarying6 = isSet6;
    if(isSet6<=0)
        return ;
    projTextCoord6 = projectorMatrix6 * modelMatrix * position;
    shadowTextCoord6 = (biasMatrix * projectorProjectionMatrix6 * projectorViewMatrix6) * modelMatrix * position;
    
    vec3 aux = projectorPos6 - vec3(modelMatrix * position);
    projectorDirVarying6 = normalize(aux);
    distVarying6 = length(aux);
    
    vec3 L = projectorPos6 - position.xyz;
    vec3 V = -position.xyz;
    halfVector6 = normalize(L+V);
}

//seventh projector's variables
uniform mat4 projectorMatrix7;
uniform mat4 projectorProjectionMatrix7;
uniform mat4 projectorViewMatrix7;
uniform vec3 projectorPos7;
uniform float isSet7;
out vec4 projTextCoord7;
out vec4 shadowTextCoord7;
out vec3 projectorDirVarying7;
out float distVarying7;
out vec3 halfVector7;
out float isSetVarying7;
void setProjector7() {
    isSetVarying7 = isSet7;
    if(isSet7<=0)
        return ;
    projTextCoord7 = projectorMatrix7 * modelMatrix * position;
    shadowTextCoord7 = (biasMatrix * projectorProjectionMatrix7 * projectorViewMatrix7) * modelMatrix * position;
    
    vec3 aux = projectorPos7 - vec3(modelMatrix * position);
    projectorDirVarying7 = normalize(aux);
    distVarying7 = length(aux);
    
    vec3 L = projectorPos7 - position.xyz;
    vec3 V = -position.xyz;
    halfVector7 = normalize(L+V);
}

void main() {
    setProjector1();
    setProjector2();
    setProjector3();
    //setProjector4();
    //setProjector5();
    //setProjector6();
    //setProjector7();

    fragPos = vec3(modelMatrix * position);
    
    gl_Position = modelViewProjectionMatrix * position;
    
    normalVarying = normalize(normal);
}
