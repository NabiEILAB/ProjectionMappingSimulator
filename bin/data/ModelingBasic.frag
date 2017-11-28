
#version 150

//Shared variables
out vec4 fragColor;
in vec3 normalVarying;
uniform vec3 lightPos;
in vec3 fragPos;
uniform float isModelingSelected;
void basicTexturing() {
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normalVarying, lightDir), 0.0);
    vec3 lightColor = vec3(0.5, 0.5, 0.5);
    
    if(isModelingSelected >= 0.0)
        lightColor = vec3(0.0, 1.0, 0.0);
    
    vec3 diffuse = diff * lightColor;
    fragColor = vec4(diffuse, 1.0);
}

void main() {
    basicTexturing();
}




