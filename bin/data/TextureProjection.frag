
#version 150

//Shared variables
//uniform int runningProjector[10];
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
uniform float radius1;
in float isSetVarying1;
void drawProjector1() {
    if(isSetVarying1<=0)
        return ;
    
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
        if(NdotL < 1.0)
            NdotL = 1.0; //att가 1보다 커도 NdotL이 거리가 가까울때 1 아래로 내려가서 오히려 밝기값이 약해지기 때문에 1보다 작으면 1로 바꾸기
        
        //https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/ 에서 버전은 radius를 추가(코드에선 height로)
        att = 1.5 / (1.0 + (2.0 / radius1 * distVarying1) + (1.0 / (radius1 * radius1)) * (distVarying1 * distVarying1));
        att += -0.5 / (1.0 + (2.0 / radius1 * (1500.0 - distVarying1)) + (1.0 / (radius1 * radius1)) * ((1500.0 - distVarying1) * (1500.0 - distVarying1)));
        
        //기존 버전인데 모델의 크기가 가변하면 똑같이 밝기값 처리가 안되는 문제 발생
        //  att = 5000.0 / (0.1 + 0.1 * distVarying1 + 0.1 * distVarying1 * distVarying1);
        //att += -5000.0 / (0.1 + 0.1 * (1000.0 - distVarying1) + 0.1 * (1000.0 - distVarying1) * (1000.0 - distVarying1));
        
        /*float constantAttenuation = 1.0;
        float linearAttenuation = 0.22;
        float quadraticAttenuation = 0.20;
        
        if (distVarying1<70.0){
            constantAttenuation=1.0;
            linearAttenuation=0.7;
            quadraticAttenuation=1.8;
        }
        
        else if (distVarying1<130.0){
            constantAttenuation=1.0;
            linearAttenuation=0.35;
            quadraticAttenuation=0.44;
        }
        
        else if (distVarying1<200.0){
            constantAttenuation=1.0;
            linearAttenuation=0.22;
            quadraticAttenuation=0.20;
        }
        
        if (distVarying1<320.0){
            constantAttenuation=1.0;
            linearAttenuation=0.14;
            quadraticAttenuation=0.07;
        }
        if (distVarying1<500.0){
            constantAttenuation=1.0;
            linearAttenuation=0.09;
            quadraticAttenuation=0.32;
        }
        
        if (distVarying1<650.0){
            constantAttenuation=1.0;
            linearAttenuation=0.07;
            quadraticAttenuation=0.017; 
        }
        
        if (distVarying1<1000.0){
            constantAttenuation=1.0;
            linearAttenuation=0.045;
            quadraticAttenuation=0.0075; 
        }
        
        att = constantAttenuation / ((1 + linearAttenuation * distVarying1) * (1 + quadraticAttenuation * distVarying1 * distVarying1));*/
        
        lightColor += att * vec4(diffuseLight * NdotL * ambientLight, 1.0);
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

//Second projector's variables
uniform sampler2D shadowTex2;
uniform sampler2DRect projectorTex2;
in vec4 projTextCoord2;
in vec4 shadowTextCoord2;
in vec3 projectorDirVarying2;
in vec3 halfVector2;
in float distVarying2;
uniform float radius2;
in float isSetVarying2;
void drawProjector2() {
    if(isSetVarying2<=0)
        return ;
    
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
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius2 * distVarying2) + (1.0 / (radius2 * radius2)) * (distVarying2 * distVarying2));
        att += -0.5 / (1.0 + (2.0 / radius2 * (1500.0 - distVarying2)) + (1.0 / (radius2 * radius2)) * ((1500.0 - distVarying2) * (1500.0 - distVarying2)));
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

//third projector's variables
uniform sampler2D shadowTex3;
uniform sampler2DRect projectorTex3;
in vec4 projTextCoord3;
in vec4 shadowTextCoord3;
in vec3 projectorDirVarying3;
in vec3 halfVector3;
in float distVarying3;
uniform float radius3;
in float isSetVarying3;
void drawProjector3() {
    if(isSetVarying3<=0)
        return ;
    
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying3), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius3 * distVarying3) + (1.0 / (radius3 * radius3)) * (distVarying3 * distVarying3));
        att += -0.5 / (1.0 + (2.0 / radius3 * (1500.0 - distVarying3)) + (1.0 / (radius3 * radius3)) * ((1500.0 - distVarying3) * (1500.0 - distVarying3)));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector3);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord3.xyz / shadowTextCoord3.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex3, projTextCoord3);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex3, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
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

//Fourth projector's variables
uniform sampler2D shadowTex4;
uniform sampler2DRect projectorTex4;
in vec4 projTextCoord4;
in vec4 shadowTextCoord4;
in vec3 projectorDirVarying4;
in vec3 halfVector4;
in float distVarying4;
uniform float radius4;
in float isSetVarying4;
void drawProjector4() {
    if(isSetVarying4<=0)
        return ;
    
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying4), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius4 * distVarying4) + (1.0 / (radius4 * radius4)) * (distVarying4 * distVarying4));
        att += -0.5 / (1.0 + (2.0 / radius4 * (1500.0 - distVarying4)) + (1.0 / (radius4 * radius4)) * ((1500.0 - distVarying4) * (1500.0 - distVarying4)));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector4);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord4.xyz / shadowTextCoord4.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex4, projTextCoord4);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex4, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
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

//fifth projector's variables
uniform sampler2D shadowTex5;
uniform sampler2DRect projectorTex5;
in vec4 projTextCoord5;
in vec4 shadowTextCoord5;
in vec3 projectorDirVarying5;
in vec3 halfVector5;
in float distVarying5;
uniform float radius5;
in float isSetVarying5;
void drawProjector5() {
    if(isSetVarying5<=0)
        return ;
    
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying5), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius5 * distVarying5) + (1.0 / (radius5 * radius5)) * (distVarying5 * distVarying5));
        att += -0.5 / (1.0 + (2.0 / radius5 * (1500.0 - distVarying5)) + (1.0 / (radius5 * radius5)) * ((1500.0 - distVarying5) * (1500.0 - distVarying5)));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector5);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord5.xyz / shadowTextCoord5.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex5, projTextCoord5);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex5, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
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

//sixth projector's variables
uniform sampler2D shadowTex6;
uniform sampler2DRect projectorTex6;
in vec4 projTextCoord6;
in vec4 shadowTextCoord6;
in vec3 projectorDirVarying6;
in vec3 halfVector6;
in float distVarying6;
uniform float radius6;
in float isSetVarying6;
void drawProjector6() {
    if(isSetVarying6<=0)
        return ;
    
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying6), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius6 * distVarying6) + (1.0 / (radius6 * radius6)) * (distVarying6 * distVarying6));
        att += -0.5 / (1.0 + (2.0 / radius6 * (1500.0 - distVarying6)) + (1.0 / (radius6 * radius6)) * ((1500.0 - distVarying6) * (1500.0 - distVarying6)));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector6);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord6.xyz / shadowTextCoord6.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex6, projTextCoord6);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex6, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
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

//seventh projector's variables
uniform sampler2D shadowTex7;
uniform sampler2DRect projectorTex7;
in vec4 projTextCoord7;
in vec4 shadowTextCoord7;
in vec3 projectorDirVarying7;
in vec3 halfVector7;
in float distVarying7;
uniform float radius7;
in float isSetVarying7;
void drawProjector7() {
    if(isSetVarying7<=0)
        return ;
    
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying7), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius7 * distVarying7) + (1.0 / (radius7 * radius7)) * (distVarying7 * distVarying7));
        att += -0.5 / (1.0 + (2.0 / radius7 * (1500.0 - distVarying7)) + (1.0 / (radius7 * radius7)) * ((1500.0 - distVarying7) * (1500.0 - distVarying7)));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector7);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord7.xyz / shadowTextCoord7.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex7, projTextCoord7);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex7, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
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

//eighth projector's variables
/*uniform sampler2D shadowTex8;
uniform sampler2DRect projectorTex8;
in vec4 projTextCoord8;
in vec4 shadowTextCoord8;
in vec3 projectorDirVarying8;
in vec3 halfVector8;
in float distVarying8;
uniform float radius8;
in float isSetVarying8;
void drawProjector8() {
    if(isSetVarying8<=0)
        return ;
    
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying8), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius8 * distVarying8) + (1.0 / (radius8 * radius8)) * (distVarying8 * distVarying8));
        att += -0.5 / (1.0 + (2.0 / radius8 * (1500.0 - distVarying8)) + (1.0 / (radius8 * radius8)) * ((1500.0 - distVarying8) * (1500.0 - distVarying8)));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector8);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord8.xyz / shadowTextCoord8.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex8, projTextCoord8);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex8, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
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

//nighth projector's variables
uniform sampler2D shadowTex9;
uniform sampler2DRect projectorTex9;
in vec4 projTextCoord9;
in vec4 shadowTextCoord9;
in vec3 projectorDirVarying9;
in vec3 halfVector9;
in float distVarying9;
uniform float radius9;
in float isSetVarying9;
void drawProjector9() {
    if(isSetVarying9<=0)
        return ;
    
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    float shadowBias = 0.0001;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    NdotL = max(dot(normalVarying, projectorDirVarying9), 0.0);
    if(NdotL > 0.0) {
        if(NdotL < 1.0)
            NdotL = 1.0;
        //att = 5000 / (0.1 + 0.1 * distVarying2 + 0.1 * distVarying2 * distVarying2);
        //att += -5000 / (0.1 + 0.1 * (1000 - distVarying2) + 0.1 * (1000 - distVarying2) * (1000 - distVarying2));
        att = 1.5 / (1.0 + (2.0 / radius9 * distVarying9) + (1.0 / (radius9 * radius9)) * (distVarying9 * distVarying9));
        att += -0.5 / (1.0 + (2.0 / radius9 * (1500.0 - distVarying9)) + (1.0 / (radius9 * radius9)) * ((1500.0 - distVarying9) * (1500.0 - distVarying9)));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
        halfV = normalize(halfVector9);
        NdotHV = max(dot(normalVarying, halfV), 0.0);
        float shineness = 1;
        lightColor += att * specularLight * pow(NdotHV, shineness);
        
        vec4 projTexColor;
        vec3 tex_coords = shadowTextCoord9.xyz / shadowTextCoord9.w;
        
        vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
                                     vec2(0.94558609, -0.76890725),
                                     vec2(-0.094184101, -0.92938870),
                                     vec2(0.34495938, 0.29387760));
        
        float visibility = 1.0;
        projTexColor = textureProj(projectorTex9, projTextCoord9);
        for(int i=0; i<4; i++) {
            if(texture(shadowTex9, tex_coords.xy + poissonDisk[i]/800.0).r < tex_coords.z - shadowBias)
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
}*/

uniform vec3 lightPos;
in vec3 fragPos;
void basicTexturing() {
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normalVarying, lightDir), 0.0);
    vec3 lightColor = vec3(0.5, 0.5, 0.5);
    vec3 diffuse = diff * lightColor;
    fragColor = vec4(diffuse, 1.0);
}

void main() {
    basicTexturing();
    
    drawProjector1();
    drawProjector2();
    drawProjector3();
    drawProjector4();
    drawProjector5();
    drawProjector6();
    drawProjector7();
    //drawProjector8();
    //drawProjector9();
}




/*out vec4 fragColor;
in vec3 normalVarying;

//first projector's variables
uniform sampler2D shadowTex;
uniform sampler2DRect projectorTex;
in vec4 projTextCoord;
in vec4 shadowTextCoord;
in vec3 projectorDirVarying;
in vec3 halfVector;
in float distVarying;

void main() {
    vec3 n,halfV;
    float NdotL, NdotHV;
    float spotEffect;
    float att;
    vec3 ambientLight = vec3(1.0, 1.0, 1.0);
    vec3 diffuseLight = vec3(1.0, 1.0, 1.0);
    vec4 specularLight = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    float shadowBias = 0.0001;
    
    NdotL = max(dot(normalVarying, projectorDirVarying), 0.0);
    if(NdotL > 0.0) {
        att = 5000 / (0.1 + 0.1 * distVarying + 0.1 * distVarying * distVarying);
        att += -5000 / (0.1 + 0.1 * (1000 - distVarying) + 0.1 * (1000 - distVarying) * (1000 - distVarying));
        lightColor += att * vec4(NdotL * diffuseLight, 1.0);
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
}*/

////////////// 응 안돼 //////////////




