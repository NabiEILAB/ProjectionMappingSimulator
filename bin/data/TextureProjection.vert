
#version 150

uniform mat4 projectorMatrix;
uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4  position;
in vec2  texcoord;
in vec4  color;
in vec3  normal;

out vec3 eyeNormal;
out vec4 eyePosition;
out vec4 projTextCoord;

void main(){
	eyeNormal = normalize( normalMatrix * vec4(normal, 1.0) ).xyz;
    eyePosition = modelViewMatrix * position;
    
    projTextCoord = projectorMatrix * modelMatrix * position;
	gl_Position = modelViewProjectionMatrix * position;
}

