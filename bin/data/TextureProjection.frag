
#version 150

//Shared variables
out vec4 fragColor;
in vec3 normalVarying;

//first projector's variables
uniform sampler2D shadowTex1;
uniform sampler2DRect projectorTex1;
in vec4 projTextCoord1;
in vec4 shadowTextCoord1;
in vec3 projectorDirVarying1;
in vec3 halfVector1;
in float distVarying1;
void drawProjector1() {
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    float shadowBias = 0.0001;
    
    NdotL = max(dot(normalVarying, projectorDirVarying1), 0.0);
    if(NdotL > 0.0) {
        att = 50000 / (0.1 + 0.1 * distVarying1 + 0.1 * distVarying1 * distVarying1);
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector1);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord1.xyz / shadowTextCoord1.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex1, projTextCoord1);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex1, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
                visibility -= 0.25;
        }
        
        vec4 color = lightColor * projTexColor;
        color.x *= visibility;
        color.y *= visibility;
        color.z *= visibility;
        color.w = 1.0;
        fragColor += color;
    }
    else {
        fragColor += vec4(0.0, 0.0, 0.0, 1.0);
    }
}

//Second projector's variables
uniform sampler2D shadowTex2;
uniform sampler2DRect projectorTex2;
in vec4 projTextCoord2;
in vec4 shadowTextCoord2;
in vec3 projectorDirVarying2;
in vec3 halfVector2;
in float distVarying2;
void drawProjector2() {
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying2), 0.0);
    if(NdotL > 0.0) {
        att = 50000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector2);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord2.xyz / shadowTextCoord2.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex2, projTextCoord2);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex2, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
                visibility -= 0.25;
        }
        
        vec4 color = lightColor * projTexColor;
        color.x *= visibility;
        color.y *= visibility;
        color.z *= visibility;
        color.w = 1.0;
        fragColor += color;
    }
    else {
        fragColor += vec4(0.0, 0.0, 0.0, 1.0);
    }
}

//third... fourth...

void main() {
    drawProjector1();
    drawProjector2();
}
