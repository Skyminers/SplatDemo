#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 normal;
in vec3 worldPos;
in float ourAlpha;
in vec2 NoiseTexCoord;
in vec4 fragPosLightSpace;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D NoiseTexture;
uniform sampler2D shadowMap;

#define ColorThreshold 0.0
#define EdgeThreshold 0.5
#define SecondThreshold -0.3
#define NoiseThreshold 0.7

float ShadowCalculation(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    //return closestDepth;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main() {
    float globalAlpha = 0.3;
    vec3 global = lightColor * globalAlpha;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - worldPos);
    vec3 viewDir = normalize(cameraPos - worldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;

    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    vec3 specular = spec * lightColor;

    /*if(ourAlpha > 0.3){
        specular = 0.8 * specular;
    }else{
        specular = 0.3 * specular;
    }*/
    //specular = vec3(0.0f, 0.0f, 0.0f);
    specular = max(0.0, ourAlpha-0.6) * specular;

    
    // ourAlpha 表示涂色强度，用这个值来进行噪声混合
    vec4 Noise = texture(NoiseTexture, NoiseTexCoord); 
    float NoiseValue = Noise.x;
    //float NoiseValue = 0.5;
    float EdgeDelta = ourAlpha - NoiseValue;
    vec3 resultColor = ourColor;
    vec3 groundColor = vec3(0.6f,0.6f,0.6f);
    if(EdgeDelta>=ColorThreshold) {
        //if(EdgeDelta>EdgeThreshold) 
            resultColor = ourColor*0.9+vec3(1,1,1)*NoiseValue*0.1;
        //else 
        //    resultColor = ourColor*ourAlpha+vec3(1,1,1)*NoiseValue*0.05+groundColor*(0.95-ourAlpha);
    }
    else if(EdgeDelta>=SecondThreshold) {
        if(NoiseValue>NoiseThreshold) 
            resultColor = ourColor*ourAlpha+vec3(1,1,1)*NoiseValue*0.1+groundColor*(1.0-ourAlpha-NoiseValue*0.1);
        else 
            resultColor = groundColor;//take ground texture
    }
    else {//take ground texture
        resultColor = groundColor;
    }
    // Calculate the shadow
    float shadow = ShadowCalculation(fragPosLightSpace);

    vec3 result = (global + (1.0 - shadow) * (diffuse + specular)) * resultColor;
    FragColor = vec4(result, 1);//vec4(1.0, 0.5, 0.2, 1.0);
    //FragColor = vec4(shadow, shadow, shadow, 1.0);
    //FragColor = vec4(1,0,0,1);
}
