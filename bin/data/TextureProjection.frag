
#version 150

//Shared variables
out vec4 fragColor;
in vec3 normalVarying;
in vec3 pixelPositionVarying;

//projector's variables
uniform sampler2D shadowTex;
uniform sampler2DRect projectorTex;
in vec4 projTextCoord;
in vec4 shadowTextCoord;
in vec3 projectorDirVarying;
in vec3 projectorPosVarying;
in vec3 halfVector;
uniform float radius;
void drawProjector() {
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    float dist = distance(pixelPositionVarying, projectorPosVarying);
    
    NdotL = max(dot(normalVarying, projectorDirVarying), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        
        att = 2.0 / (1.0 + (2.0 / radius * dist) + (1.0 / (radius * radius)) * (dist * dist));
        att -= 1.0 / (1.0 + (2.0 / radius * (1500.0 - dist)) + (1.0 / (radius * radius)) * ((1500.0 - dist) * (1500.0 - dist)));
        
        lightColor += att * vec4(diffuseLight * NdotL * ambientLight, 1.0);
        halfV = normalize(halfVector);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        

        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord.xyz / shadowTextCoord.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float minOffset = 0.00001;
        float maxOffset = 0.0001;
        float offsetMod = 1.0 - clamp(NdotL, 0.0, 1.0);
        float shadowBias = minOffset + maxOffset * offsetMod;
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex, projTextCoord);
        
        for(int i=0; i<4; i++) {
            if(texture(shadowTex, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
                visibility -= 0.25;
        }
        
        vec4 color = lightColor * projTexColor;
        color.x *= visibility;
        color.y *= visibility;
        color.z *= visibility;
        color.w = 1.0;
        
        if(color.x < 0)
            color.x = 0;
        if(color.y < 0)
            color.y = 0;
        if(color.z < 0)
            color.z = 0;
        
        fragColor += color;
    }
    else {
        fragColor += vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void main() {
    drawProjector();
}




