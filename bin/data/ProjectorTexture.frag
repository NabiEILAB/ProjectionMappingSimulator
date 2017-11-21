
#version 150

uniform vec3 lightPos;
uniform float isSelected;
out vec4 fragColor;
in vec3 normalVarying;
in vec3 fragPos;

void basicTexturing() {
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normalVarying, lightDir), 0.0);
    vec3 lightColor = vec3(0.5, 0.5, 0.5);
    if(isSelected!=0) {
        lightColor = vec3(0.2, 0.8, 0.2);
    }
    vec3 diffuse = diff * lightColor;
    fragColor = vec4(diffuse, 1.0);
}

void main() {
    basicTexturing();
}



