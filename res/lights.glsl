struct Light {
    vec3 position;
    vec3 color;
    vec3 direction;
    float radius;
    float attenuiation;
    bool shadow;
    mat4 lightSpaceMatrix;
};

struct DirLight {
    vec3 direction;
    vec3 specular;
    vec3 color;
};

uniform Light lights[12];
uniform int numOfLights = 0;
uniform sampler2D ShadowMap;
//uniform DirLight MainLight = {vec3(0,-1,0),vec3(.3),vec3(.2)};
uniform vec3 ambientLuminance = vec3(0.2,0.2,0.2);


float ShadowCalculation(sampler2D map, Light l,vec3 normal, vec3 fragPos, vec4 fragPosLightSpace) {

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(map, projCoords.xy).r;
    float currentDepth = projCoords.z;

    vec3 lightDirection = normalize(l.position-fragPos);

    float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);

    float shadow = currentDepth-bias > closestDepth  ? 0.4 : 0.0;
    return shadow;
}

int getNumberOfLights() {
    return numOfLights;
}

float linearAttenuiation(float t, float rad) {
    return max(0.0,min(1-t*rad,1.0));
}

Light getLight(int index, vec3 position) {
    Light l = lights[index];
    vec3 dir = l.position - position;
    l.direction = normalize(dir);
    if(index > 0)
        l.attenuiation = linearAttenuiation(length(dir),l.radius);
    else
        l.attenuiation = 1;
    return l;
}

Light getMainLight() {
    return lights[0];
}

float getMainLighthadowValue(vec3 fragPos, vec3 normal) {
    Light main = getMainLight();
    return ShadowCalculation(ShadowMap,main,normal,fragPos,main.lightSpaceMatrix*vec4(fragPos,1));
}

vec3 lightLambertian(vec3 color, vec3 direction, vec3 normal) {
    return color*max(dot(normal,direction),0);
}

vec3 MainLightLambertian(vec3 color, vec3 direction, vec3 normal) {
    return color*max(dot(normal,direction),0);
}

float fresnel(vec3 viewDirection, vec3 normal) {
    return 1- dot(viewDirection, normal);
}

vec3 lightSpecular(vec3 color, vec3 direction, float shininess, vec3 normal, vec3 viewDirection) {
    vec3 reflection = reflect(-direction,normal);
    return color*pow(max(dot(reflection,viewDirection),0),shininess)*max(sign(dot(normal,direction)),0);
}

vec3 calculateDirectionalLightSurfaceColor(DirLight light, vec3 normal, vec3 viewDir) {
    DirLight l = light;
    vec3 lightDirection = normalize(-l.direction);
    return lightLambertian(l.color, lightDirection, normal);
}

float calculateShadow() {
    /*
    float shadow = 0;
    for(int i = 0; i < numOfLights; i++) {
        Light l = lights[i];
        if(l.shadow) {
            shadow = ShadowCalculation(shadowMaps,l,l.lightSpaceMatrix*vec4(_fragPos,1.0));
        }
    }
    return shadow;
    */
    return 1.0;
}