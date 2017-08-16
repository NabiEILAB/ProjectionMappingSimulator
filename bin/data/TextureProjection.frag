
#version 150

//User input sampler
uniform sampler2D shadowTex;
uniform sampler2DRect projectorTex;

//Varying variables from vert
in vec4 projTextCoord;
in vec4 shadowTextCoord;
in vec3 projectorPosVarying;
in vec3 positionVarying;
in vec3 normalVarying;

out vec4 fragColor;

void main() {
    vec3 norm = normalize(normalVarying);
    vec3 projectorDir = normalize(projectorPosVarying - positionVarying);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;
    float diff = max(dot(norm, projectorDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec4 projTexColor;
    vec3 tex_coords = shadowTextCoord.xyz / shadowTextCoord.w;
    float depth = texture(shadowTex, tex_coords.xy).r;
    
    float shadowBias = 0.00015;
    if(depth < tex_coords.z - shadowBias) {
        projTexColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else {
        projTexColor = textureProj(projectorTex, projTextCoord);
    }
    
    fragColor = (vec4(ambient,1.0) + vec4(diffuse,1.0)) * projTexColor;
}
