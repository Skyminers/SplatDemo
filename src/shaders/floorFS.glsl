#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 normal;
in vec3 worldPos;
in float ourAlpha;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

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

    if(ourAlpha > 0.3){
        specular = 0.8 * specular;
    }else{
        specular = 0.3 * specular;
    }

    vec3 result = (global + diffuse + specular) * ourColor;
    // TODO: ourAlpha 表示涂色强度，用这个值来进行噪声混合
    FragColor = vec4(result, 1);//vec4(1.0, 0.5, 0.2, 1.0);
}
