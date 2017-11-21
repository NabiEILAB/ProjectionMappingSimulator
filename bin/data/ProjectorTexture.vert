#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
in vec4 position;
in vec3 normal;
out vec3 normalVarying;
out vec3 fragPos;

void main() {
    fragPos = vec3(modelMatrix * position);
    
    gl_Position = modelViewProjectionMatrix * position;
    
    normalVarying = normalize(normal);
}

