#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec3 normal;
in vec3 worldPos;
in float ourAlpha;
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D NoiseTexture;

#define ColorThreshold 0.5
#define EdgeThreshold 0.9
#define SecondThreshold 0.2
#define NoiseThreshold 0.5

float cookTorranceSpec(vec3 lightDir, vec3 viewDir, vec3 surfaceNormal, float r, float f);
float beckmannDistribution(float NdotH, float f);

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
    specular = max(0.0, ourAlpha-0.45) * specular;

    
    // TODO: ourAlpha 表示涂色强度，用这个值来进行噪声混合
    //vec4 Noise = texture(NoiseTexture, TexCoord);
    float NoiseValue = 0.5;//Noise.x;
    //float EdgeDelta = 
    vec3 resultColor = ourColor;
    vec3 groundColor = vec3(0.6f,0.6f,0.6f);
    if(ourAlpha>ColorThreshold) {
        if(ourAlpha>EdgeThreshold) 
            resultColor = ourColor*0.9+vec3(1,1,1)*NoiseValue*0.1;
        else 
            resultColor = ourColor*ourAlpha+vec3(1,1,1)*NoiseValue*0.1+groundColor*(0.9-ourAlpha);
    }
    else if(ourAlpha>SecondThreshold) {
        if(NoiseValue>NoiseThreshold) 
            resultColor = ourColor*ourAlpha+vec3(1,1,1)*NoiseValue*0.1+groundColor*(0.9-ourAlpha);
        else 
            resultColor = groundColor;//take ground texture
    }
    else {//take ground texture
        resultColor = groundColor;
    }
    vec3 result = (global + diffuse + specular) * resultColor;
    FragColor = vec4(result, 1);//vec4(1.0, 0.5, 0.2, 1.0);
}
